#pragma once

#include <boost/asio.hpp>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/log/trivial.hpp>

class ClientNetwork {
 public:
  ClientNetwork();
  ~ClientNetwork();
  void SendJSON(const boost::property_tree::ptree &jsonRequest);
  boost::property_tree::ptree ReceiveJSON();
  void Connect(const std::string &localhost = "localhost", int port = 5555);

 private:
  boost::asio::io_service _service;
  boost::asio::ip::tcp::socket _socket;
};

