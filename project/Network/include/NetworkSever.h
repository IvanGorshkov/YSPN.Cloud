#ifndef _NETWORKSEVER_H
#define _NETWORKSEVER_H

#include <string>
#include "Connection.h"
#include "UserSession.h"
#include <queue>

class NetworkSever {
public:
    std::string host;

    Connection GetConnect();

    void RunNetworkServer();

    void ConnectionLoop();

    void GetConnection();

private:
    int _port;
    Connection _network;

    // boost::asio::ip::tcp::socket _socket;
    boost::asio::ip _ip;

    std::queue<UserSession> _connections;

    void startAccept();

    void onAccept();
};

#endif //_NETWORKSEVER_H