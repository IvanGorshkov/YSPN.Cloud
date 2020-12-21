#include "Watcher.h"

#include <utility>

namespace bfs = boost::filesystem;

Watcher::Watcher() :
    _eventMask(IN_ALL_EVENTS),
    _lastEventTime(std::chrono::steady_clock::now()),
    _inotifyFd(0),
    _stopped(false),
    _isReanameEvent(false),
    _eventBuffer(MAX_EVENTS * (EVENT_SIZE + 16), 0) {
  _inotifyFd = inotify_init1(IN_NONBLOCK);
  if (_inotifyFd == -1) {
    throw InotifyInitError();
  }

  _epollFd = epoll_create1(0);
  if (_epollFd == -1) {
    throw EpollInitError();
  }

  _inotifyEpollEvent.events = EPOLLIN | EPOLLET;
  _inotifyEpollEvent.data.fd = _inotifyFd;
  if (epoll_ctl(_epollFd, EPOLL_CTL_ADD, _inotifyFd, &_inotifyEpollEvent) == -1) {
    throw EpollFDError();
  }

}

Watcher::~Watcher() {
  epoll_ctl(_epollFd, EPOLL_CTL_DEL, _inotifyFd, nullptr);
}

void Watcher::Run(const boost::filesystem::path &path, std::function<void(Notification)> callBack) {
  try {
    watchDirectory(path);
  } catch (std::exception &e) {
    _exceptionQueue.push(std::current_exception());
    Stop();
  }

//  auto events = {Event::_moved_to,
//                 Event::_moved_from,
//                 Event::_create,
//                 Event::_remove,
//                 Event::_modify,
//                 Event::_close_write};
//  for (auto event : events) {
//    _eventMask = _eventMask | static_cast<std::uint32_t>(event);
//    _eventCallbacks[event] = callBack;
//  }
    _eventCallback = std::move(callBack);


  while (true) {
    if (hasStopped()) {
      break;
    }
    runOnce();
  }
}

void Watcher::runOnce() {
  auto newEvent = getNextEvent();
  if (!newEvent) {
    return;
  }

  Event currentEvent = static_cast<Event>(newEvent->mask);

  switch(currentEvent){
    case 256:
      currentEvent = Event::_create;
      break;
    case 64:
      currentEvent = Event::_moved_from;
      break;
    case 128:
      currentEvent = Event::_moved_to;
      break;
    case 512:
      currentEvent = Event::_remove;
      break;
    case 8:
      currentEvent = Event::_modify;
      break;
    default:
      currentEvent = Event ::_ignored;
      break;
  }

  Notification notification{currentEvent, newEvent->path, newEvent->eventTime};

//  for (auto &eventAndCallback : _eventCallbacks) {
//    auto &event = eventAndCallback.first;
//    auto &callbackFunc = eventAndCallback.second;
//
//    if (event == currentEvent) {
  if(notification.event != Event::_ignored)
    _eventCallback(notification);

      //callbackFunc(notification);
      return;
   // }
 // }

}

bool Watcher::hasStopped() const {
  return _stopped;
}

void Watcher::Stop() {
  _stopped = true;
}

void Watcher::watchFile(bfs::path file) {
  if (bfs::exists(file)) {
    int wd = 0;
    wd = inotify_add_watch(_inotifyFd, file.string().c_str(), _eventMask);
    if (wd == -1) {
      throw InotifyAddWatchError(file.string());
    }
    _directorieMap.left.insert({wd, file});
  } else {
    throw WrongPath(file.string());
  }
}

boost::optional<FileSystemEvent> Watcher::getNextEvent() {

  std::vector<FileSystemEvent> newEvents;

  while (_eventQueue.empty() && !_stopped) {
    auto length = readEventsIntoBuffer(_eventBuffer);
    readEventsFromBuffer(_eventBuffer.data(), length, newEvents);

    for (auto eventIt = newEvents.begin(); eventIt < newEvents.end();) {
      FileSystemEvent currentEvent = *eventIt;
      _lastEventTime = currentEvent.eventTime;
      _eventQueue.push(currentEvent);
      eventIt++;
    }
  }
  if (_stopped) {
    return boost::none;
  }
  auto event = _eventQueue.front();
  _eventQueue.pop();
  return event;
}

ssize_t Watcher::readEventsIntoBuffer(std::vector<uint8_t> &eventBuffer) {
  ssize_t length = 0;
  auto timeout = -1;
  auto nFdsReady = epoll_wait(_epollFd, _epollEvents, 1, timeout);

  if (nFdsReady == -1) {
    return length;
  }
  for (auto n = 0; n < nFdsReady; ++n) {

    length = read(_epollEvents[n].data.fd, eventBuffer.data(), eventBuffer.size());
    if (length == -1) {
      break;
    }
  }
  return length;
}

void Watcher::readEventsFromBuffer(
    uint8_t *buffer, int length, std::vector<FileSystemEvent> &events) {
  int i = 0;
  while (i < length) {
    inotify_event *event = ((struct inotify_event *) &buffer[i]);

    auto path = wdToPath(event->wd);

    if (event->name[0] == '.') {
      i += EVENT_SIZE + event->len;
      //_directorieMap.left.erase(event->wd);
      continue;
    }

    if (bfs::is_directory(path)) {
      path = path / std::string(event->name);
    }
    if (path.extension() == "" || path.extension() == ".tmp" || path.extension().string().back() == '~') {
      i += EVENT_SIZE + event->len;
      continue;
    }
    //printf(path.extension().string().c_str());
    if (bfs::is_directory(path)) {
      event->mask |= IN_ISDIR;
    }
    FileSystemEvent fsEvent(event->wd, event->mask, path, std::chrono::steady_clock::now());

    if (!fsEvent.path.empty()) {
      events.push_back(fsEvent);
    }
    i += sizeof(inotify_event) + event->len;
  }
}

void Watcher::watchDirectory(bfs::path path) {

  std::vector<bfs::path> paths;

  if (bfs::exists(path)) {
    paths.push_back(path);
    if (bfs::is_directory(path)) {
      boost::system::error_code errorCode;
      bfs::recursive_directory_iterator begin(path, bfs::symlink_option::none, errorCode);
      bfs::recursive_directory_iterator end;
      std::copy_if(begin, end, std::back_inserter(paths), [](const bfs::path &path) {
        return bfs::is_directory(path);
      });
    }
  } else {
    throw WrongPath(path.string());
  }

  for (auto &new_path : paths)
    watchFile(new_path);
}

void Watcher::UnwatcFile(const bfs::path &file) {
  removeWatch(_directorieMap.right.at(file));
}

void Watcher::removeWatch(int wd) const {
  int error_flag = inotify_rm_watch(_inotifyFd, wd);
  if (error_flag == -1) {
    throw InotifyRemoveWatchError();
  }
}

boost::filesystem::path Watcher::wdToPath(int wd) {
  return _directorieMap.left.at(wd);
}

std::exception_ptr Watcher::GetLastException() {
  std::exception_ptr e;
  if (!_exceptionQueue.empty()) {
    e = _exceptionQueue.front();
    _exceptionQueue.pop();
  }
  return e;
}