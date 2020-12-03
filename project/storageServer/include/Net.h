#pragma once

#include <queue>
#include <iostream>
#include <boost/property_tree/ptree.hpp>

class UserSession {
 public:
  explicit UserSession(int id)
      : id(id) {
  }

  int GetUserId() {
    return id;
  }

 private:
  int id;
};

class NetworkServer {
 public:
  std::shared_ptr<std::pair<UserSession, boost::property_tree::ptree>> GetConnection() {
    if (!_request.empty()) {
      auto request = _request.front();
      _request.pop();
      return request;
    }

    return nullptr;
  }

  void PutConnection(const std::shared_ptr<std::pair<UserSession, boost::property_tree::ptree>> &responce) {
    _request.push(responce);
  }

 private:
  std::queue<std::shared_ptr<std::pair<UserSession, boost::property_tree::ptree>>> _request;
  std::queue<std::shared_ptr<std::pair<UserSession, boost::property_tree::ptree>>> _responce;
};

class MongoDB {
 public:
  void InsertChunk() {}
  void GetChunk() {}
};