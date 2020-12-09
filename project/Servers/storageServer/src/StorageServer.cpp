#include "StorageServer.h"
#include <boost/log/trivial.hpp>

StorageServer::StorageServer()
    : _config(Config::GetInstance()) {
  BOOST_LOG_TRIVIAL(debug) << "Storage server: create storage server";
}

StorageServer::~StorageServer() {
  BOOST_LOG_TRIVIAL(debug) << "Storage server: delete storage server";
  stopWorkers();
}

void StorageServer::Run() {
  BOOST_LOG_TRIVIAL(info) << "Storage server: start server";
  startWorkers();
}

void StorageServer::startWorkers() {
  _workerThreads.reserve(_config.GetStorageConfig().workersCount);

  for (int i = 0; i < _config.GetStorageConfig().workersCount; ++i) {
    _workerThreads.emplace_back(&Worker::Run,
                                std::move(Worker(std::make_shared<StorageManager>(),
                                                 std::bind(&NetworkServer::GetConnection, &networkServer))));

    BOOST_LOG_TRIVIAL(info) << "Storage server: add new worker with id = " << _workerThreads[i].get_id();
  }
}

void StorageServer::stopWorkers() {
  if (!_workerThreads.empty()) {
    for (auto &thread : _workerThreads) {
      BOOST_LOG_TRIVIAL(info) << "Storage server: join worker with id = " << thread.get_id();
      thread.join();
    }

    _workerThreads.clear();
  }
}
