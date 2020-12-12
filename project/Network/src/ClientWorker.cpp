#include "ClientWorker.h"

void ClientWorker::PutConnection(const std::shared_ptr<UserSession> &user) {
  BOOST_LOG_TRIVIAL(debug) << "ClientWorker: PutConnection";
  _usersConnections.Push(user);
}

// функция отдачи запроса воркеру
std::shared_ptr<std::pair<std::shared_ptr<UserSession>,
                          pt::ptree> > ClientWorker::SendRequestToServer() {
  BOOST_LOG_TRIVIAL(debug) << "ClientWorker: SendRequestToServer";
  if (!_request.Empty()) {
    return _request.Pop();
  }

  return nullptr;
}

// функция получения ответа от воркера
void ClientWorker::GetResponseFromServer(
    const std::shared_ptr<std::pair<std::shared_ptr<UserSession>, pt::ptree> > &response) {
  BOOST_LOG_TRIVIAL(debug) << "ClientWorker: GetResponseFromServer";
  _response.Push(response);
}

// функция получения запроса от пользователя
void ClientWorker::getRequest(const std::shared_ptr<UserSession> &user) {
  BOOST_LOG_TRIVIAL(debug) << "ClientWorker: getRequest";
  pt::ptree userRequest = user->GetRequest();

  if (!userRequest.empty()) {
    auto pairRequest = std::make_shared<std::pair<std::shared_ptr<UserSession>, pt::ptree>>(
        std::make_pair(user, userRequest));

    _request.Push(pairRequest);
  }
}

// функция отправки ответа пользователю
void ClientWorker::sendResponse() {
  BOOST_LOG_TRIVIAL(debug) << "ClientWorker: sendResponse";
  auto pairResponse = _response.Pop();
  pairResponse->first->SendResponse(pairResponse->second);
}

// воркер получения запросов пользователя
[[noreturn]] void ClientWorker::workerGetRequests() {
  BOOST_LOG_TRIVIAL(debug) << "ClientWorker: workerGetRequests";
  while (true) {
    if (_usersConnections.Empty()) {
      BOOST_LOG_TRIVIAL(debug) << "ClientWorker: sleep request";
      std::this_thread::sleep_for(std::chrono::seconds(2));
    } else {
      BOOST_LOG_TRIVIAL(debug) << "ClientWorker: get request";
      if (!_usersConnections.Empty()) {
        getRequest((_usersConnections.Pop()));
      }
    }
  }
}

// воркер отправки ответа пользователям
[[noreturn]] void ClientWorker::workerSendResponses() {
  BOOST_LOG_TRIVIAL(debug) << "ClientWorker: workerSendResponses";
  while (true) {
    if (_response.Empty()) {
      BOOST_LOG_TRIVIAL(debug) << "ClientWorker: sleep response";
      std::this_thread::sleep_for(std::chrono::seconds(2));
    } else {
      BOOST_LOG_TRIVIAL(debug) << "ClientWorker: send response";
      if (!_response.Empty()) {
        sendResponse();
      }
    }
  }
}

void ClientWorker::RunClientWorker() {
  BOOST_LOG_TRIVIAL(debug) << "ClientWorker: run client network";
  std::vector<std::thread> _workerGetRequests;
  std::vector<std::thread> _workerPutResponses;

//  auto &config = Config::GetInstance();

  size_t requestWorkersCount = 4;
  _workerGetRequests.reserve(requestWorkersCount);
  for (int i = 0; i < requestWorkersCount; ++i) {
    _workerGetRequests.emplace_back([this] { workerGetRequests(); });
  }

  size_t responceWorkersCount = 4;
  _workerPutResponses.reserve(responceWorkersCount);
  for (int i = 0; i < responceWorkersCount; ++i) {
    _workerPutResponses.emplace_back([this] { workerSendResponses(); });
  }

  for (auto &thread : _workerGetRequests) {
    thread.join();
  }

  for (auto &thread : _workerPutResponses) {
    thread.join();
  }
}
