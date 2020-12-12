#pragma once

#include <string>
#include <boost/property_tree/ptree.hpp>
#include "UserSession.h"
#include "ClientWorker.h"

namespace pt = boost::property_tree;

class NetworkSever {
 public:
  explicit NetworkSever(short port = 5555, short backlog = 1024);
  void StartServer();
  void PutResponce(const std::shared_ptr<std::pair<std::shared_ptr<UserSession>,
                                                   pt::ptree> > &response);
  std::shared_ptr<std::pair<std::shared_ptr<UserSession>, pt::ptree> > GetRequest();

 private:
  void onAccept(const std::shared_ptr<UserSession> &user, const boost::system::error_code &ec);
  void startAccept();
  void run();

 private:
  ClientWorker _queue;
  boost::asio::io_service _service;
  boost::asio::ip::tcp::acceptor _acceptor;
};
