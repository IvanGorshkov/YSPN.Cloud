#include "StorageServer.h"

StorageServer::StorageServer()
    : _isWorkingWorker(false),
      _config(Config::GetInstance()) {
  BOOST_LOG_TRIVIAL(debug) << "Storage: create storage server";
}

StorageServer::~StorageServer() {
  BOOST_LOG_TRIVIAL(debug) << "Storage: delete storage server";
  stopWorkers();
}

void StorageServer::Run() {
  BOOST_LOG_TRIVIAL(info) << "Storage: start server";
  startWorkers();
}

void StorageServer::Stop() {
  BOOST_LOG_TRIVIAL(info) << "Storage: stop server";
  stopWorkers();
}

void StorageServer::startWorkers() {
  _isWorkingWorker = true;
  _workerThreads.reserve(_config.GetStorageConfig().workersCount);

  for (int i = 0; i < _config.GetStorageConfig().workersCount; ++i) {
    Worker worker(std::bind(&NetworkServer::GetConnection, &networkServer), _isWorkingWorker);
    BOOST_LOG_TRIVIAL(info) << "Storage: add test";
    _workerThreads.emplace_back(&Worker::Run, std::move(worker));

//    _workerThreads.emplace_back(&Worker::Run,
//                                Worker(std::bind(&NetworkServer::GetConnection, &networkServer), _isWorkingWorker));
    BOOST_LOG_TRIVIAL(info) << "Storage: add new worker with id = " << _workerThreads[i].get_id();
  }
}

void StorageServer::stopWorkers() {
  _isWorkingWorker = false;

  if (!_workerThreads.empty()) {
    for (auto &thread : _workerThreads) {
      BOOST_LOG_TRIVIAL(info) << "Storage: join worker with id = " << thread.get_id();
      thread.join();
    }

    _workerThreads.clear();
  }
}
