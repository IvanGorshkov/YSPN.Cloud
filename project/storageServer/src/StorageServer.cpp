#include "StorageServer.h"

StorageServer::StorageServer(NetworkServer *_network, MongoDB *_db)
    : _networkServer(_network),
      _mongoDB(_db) {
}

void StorageServer::runNetworkServer() {
}

void StorageServer::listeningConnection() {
}

void StorageServer::onConnect() {
}

void StorageServer::uploadChunk() {
}

void StorageServer::downloadChunk() {
}


