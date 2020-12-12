#ifndef _CLIENT_H
#define _CLIENT_H

#include <boost/asio.hpp>
#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/log/trivial.hpp>

class Client {
public:
    Client();

    ~Client();

    void SendJSON(const boost::property_tree::ptree &jsonRequest);

    boost::property_tree::ptree ReceiveJSON();

    void Connect(const std::string &localhost = "localhost", int port = 5555);

private:
    boost::asio::io_service _service;
    boost::asio::ip::tcp::socket _socket;
};

#endif // _CLIENT_H
