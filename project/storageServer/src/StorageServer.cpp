#include "StorageServer.h"

StorageServer::StorageServer(MongoDB *_db)
    : _mongoDB(_db) {
}

void StorageServer::runNetworkServer() {
}

void StorageServer::listeningConnection() {
}

void StorageServer::onConnect(UserSession *session) {
}

void StorageServer::uploadChunk() {
}

void StorageServer::downloadChunk() {
}


