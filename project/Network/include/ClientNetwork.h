#ifndef _CLIENTNETWORK_H
#define _CLIENTNETWORK_H

#include "SafeQueue.h"
#include "UserSession.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/asio.hpp>
#include <functional>
#include <iostream>
#include <thread>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/log/trivial.hpp>

class ClientNetwork {
public:

    void RunClientNetwork();

    std::shared_ptr<std::pair<std::shared_ptr<UserSession>, boost::property_tree::ptree> > SendRequestToServer();

    void GetResponseFromServer(const std::shared_ptr<std::pair<std::shared_ptr<UserSession>, boost::property_tree::ptree> > &response);

    void PutConnection(const std::shared_ptr<UserSession>& user);

    ClientNetwork();

    ~ClientNetwork();

private:

    void sendResponse();

    void getRequest(const std::shared_ptr<UserSession> &user);

    [[noreturn]] void workerSendResponses();

    [[noreturn]] void workerGetRequests();

    SafeQueue<std::shared_ptr<UserSession>> _usersConnections;

    SafeQueue<std::shared_ptr<std::pair<std::shared_ptr<UserSession>, boost::property_tree::ptree> > > _request;

    SafeQueue<std::shared_ptr<std::pair<std::shared_ptr<UserSession>, boost::property_tree::ptree> > > _response;

};

#endif //_CLIENTNETWORK_H