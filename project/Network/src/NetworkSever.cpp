#include "NetworkSever.h"

NetworkSever::NetworkSever(short port)
        : _acceptor(_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {
    //StartServer();
    _acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    std::thread threadServerRunning(&ClientNetwork::RunClientNetwork, &_queue);
    threadServerRunning.detach();
}

void NetworkSever::StartServer() {
    BOOST_LOG_TRIVIAL(debug) << "NetworkSever: start server";
    _acceptor.listen(1024);
    std::cerr << "listen on port: 5555" << std::endl;
    startAccept();
    run();
}

void NetworkSever::run() {
    BOOST_LOG_TRIVIAL(debug) << "NetworkSever: run service";
    boost::system::error_code ec;
    _service.run(ec);
    if (ec){
        BOOST_LOG_TRIVIAL(fatal) << "NetworkSever: error run service: " << ec.message();
        return;
    }
}

void NetworkSever::startAccept() {
    BOOST_LOG_TRIVIAL(debug) << "NetworkSever: start accept";
    auto user = std::make_shared<UserSession>(_service);
    _acceptor.async_accept(user->Sock(),
                           boost::bind(&NetworkSever::onAccept, this, user,
                                       boost::asio::placeholders::error));
}

void NetworkSever::onAccept(const std::shared_ptr<UserSession> &user, const boost::system::error_code &e) {
    if (e) {
        BOOST_LOG_TRIVIAL(error) << "NetworkSever: error accept user" << e.message();
        return;
    }
    _queue.PutConnection(user);
    startAccept();
}

void NetworkSever::GetResponseFromWorker(
        const std::shared_ptr<std::pair<std::shared_ptr<UserSession>, boost::property_tree::ptree> > &response) {
    BOOST_LOG_TRIVIAL(debug) << "NetworkSever: get response from worker";
    _queue.GetResponseFromServer(response);
}

std::shared_ptr<std::pair<std::shared_ptr<UserSession>, boost::property_tree::ptree> > NetworkSever::SendRequestToWorker() {
    BOOST_LOG_TRIVIAL(debug) << "NetworkSever: send response to worker";
    return _queue.SendRequestToServer();
}



