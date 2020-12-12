#include "ClientNetwork.h"

ClientNetwork::ClientNetwork() = default;

ClientNetwork::~ClientNetwork() = default;

void ClientNetwork::PutConnection(const std::shared_ptr<UserSession>& user) {
    BOOST_LOG_TRIVIAL(debug) << "ClientNetwork: put user connection";
    _usersConnections.Push(user);
}

// функция отдачи запроса воркеру
std::shared_ptr<std::pair<std::shared_ptr<UserSession>, boost::property_tree::ptree> > ClientNetwork::SendRequestToServer() {
    BOOST_LOG_TRIVIAL(debug) << "ClientNetwork: return request to worker";
    if (!_request.Empty())
        return _request.Pop();
    return nullptr;
}

// функция получения ответа от воркера
void ClientNetwork::GetResponseFromServer(
        const std::shared_ptr<std::pair<std::shared_ptr<UserSession>, boost::property_tree::ptree> > &response) {
    BOOST_LOG_TRIVIAL(debug) << "ClientNetwork: get response from worker";
    _response.Push(response);
}

// функция получения запроса от пользователя
void ClientNetwork::getRequest(const std::shared_ptr<UserSession> &user) {
    BOOST_LOG_TRIVIAL(debug) << "ClientNetwork: get request from user";
    boost::property_tree::ptree userRequest = user->GetRequest();

    if (!userRequest.empty()) {
        auto pairRequest = std::make_shared<std::pair<std::shared_ptr<UserSession>, boost::property_tree::ptree>>(
                std::make_pair(user, userRequest));
        //_request.Push(pairRequest);
        _response.Push(pairRequest); // testVersion
    }
}

// функция отправки ответа пользователю
void ClientNetwork::sendResponse() {
    BOOST_LOG_TRIVIAL(debug) << "ClientNetwork: send response to user";
    auto pairResponse = _response.Pop();
    pairResponse->first->SendResponse(pairResponse->second);
}

// воркер получения запросов пользователя
[[noreturn]] void ClientNetwork::workerGetRequests() {
    BOOST_LOG_TRIVIAL(debug) << "ClientNetwork: worker get requests";
    while (true) {
        if (_usersConnections.Empty())
            sleep(1);
        else {
            std::vector<std::thread> threads;
            for (int i = 0; i < 4; ++i)
                if (!_usersConnections.Empty()) {
                    threads.emplace_back(
                            [this] { getRequest((_usersConnections.Pop())); });
                }
            for (auto &thread : threads)
                thread.join();
        }
    }
}

// воркер отправки ответа пользователям
[[noreturn]] void ClientNetwork::workerSendResponses() {
    BOOST_LOG_TRIVIAL(debug) << "ClientNetwork: worker send responses";
    while (true) {
        if (_response.Empty())
            sleep(1);
        else {
            std::vector<std::thread> threads;
            for (int i = 0; i < 4; ++i)
                if (!_response.Empty()) {
                    threads.emplace_back([this] { sendResponse(); });
                }
            for (auto &thread : threads)
                thread.join();
        }
    }
}

void ClientNetwork::RunClientNetwork() {
    BOOST_LOG_TRIVIAL(debug) << "ClientNetwork: run client network";
    std::vector<std::thread> threads;
    threads.emplace_back([this] { workerGetRequests(); });
    threads.emplace_back([this] { workerSendResponses(); });
    for (auto &thread : threads)
        thread.join();
}














