#ifndef _USERSESSION_H
#define _USERSESSION_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <functional>
#include <iostream>
#include <boost/log/trivial.hpp>

class UserSession
        : public std::enable_shared_from_this<UserSession> {
public:

    explicit UserSession(boost::asio::io_service &io);

    boost::asio::ip::tcp::socket &Sock();

    boost::property_tree::ptree GetRequest();

    void SendResponse(const boost::property_tree::ptree &jsonSend);


private:
    boost::asio::ip::tcp::socket _socket;

    char _readBuf[1024];

    boost::property_tree::ptree _jsonReceive;

    void receiveRequest();

    void receiveResponse(const boost::property_tree::ptree &jsonSend);
};

#endif //_USERSESSION_H