#include "SyncServer.h"

SyncServer::SyncServer(NetworkServer *_network, MetaDataDB *_db)
    : _networkServer(_network),
      _metaDataDB(_db) {
}

void SyncServer::runNetworkServer() {
}

void SyncServer::listeningConnection() {
}

void SyncServer::onConnect() {
}

void SyncServer::uploadFileMeta() {
}

void SyncServer::downloadFileMeta() {
}

void SyncServer::uploadChunkMeta() {
}

void SyncServer::downloadChunkMeta() {
}

void SyncServer::uploadFileChunksMeta() {
}

void SyncServer::downloadFileCHunksMeta() {
}

void SyncServer::findFiles() {
}