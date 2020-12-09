#pragma once

#include <string>
#include <vector>
#include "SerializerChunk.h"
#include "SerializerUserChunk.h"
#include <chrono>
#include <iostream>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/types.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/string/view_or_value.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>

using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::finalize;

class MongoDB {
 public:
  static MongoDB& shared();
  ~MongoDB();
  void InsertChunk(const std::vector<Chunk>& chunks) const;
  std::vector<Chunk> GetChunk(const std::vector<UserChunk>& userChunks) const;
 private:
  MongoDB();
  MongoDB(const MongoDB& mongo_db) = delete;
  MongoDB& operator=(const MongoDB& mongo_db) = delete;

  mongocxx::instance _instance{};
  mongocxx::database _database;
  mongocxx::uri _uri{};
  mongocxx::client _client;
};
