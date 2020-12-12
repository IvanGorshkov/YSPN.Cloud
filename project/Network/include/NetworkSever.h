#ifndef _NETWORKSEVER_H
#define _NETWORKSEVER_H

#include <string>
#include "UserSession.h"
#include "ClientNetwork.h"

class NetworkSever {
public:

    explicit NetworkSever(short port = 5555);

    void StartServer();

    void GetResponseFromWorker(const std::shared_ptr<std::pair<std::shared_ptr<UserSession>, boost::property_tree::ptree> > &response);

    std::shared_ptr<std::pair<std::shared_ptr<UserSession>, boost::property_tree::ptree> > SendRequestToWorker();

private:
    ClientNetwork _queue;

    boost::asio::io_service _service;

    boost::asio::ip::tcp::acceptor _acceptor;

    void onAccept(const std::shared_ptr<UserSession> &user, const boost::system::error_code &e);

    void startAccept();

    void run();

};

#endif //_NETWORKSEVER_H