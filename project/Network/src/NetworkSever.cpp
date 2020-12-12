#include "NetworkSever.h"

NetworkSever::NetworkSever(short port, short backlog)
    : _acceptor(_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {
  BOOST_LOG_TRIVIAL(info) << "NetworkSever: listen on port " << port << ", backlog: " << backlog;

  _acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
  _acceptor.listen(backlog);
}

void NetworkSever::StartServer() {
  BOOST_LOG_TRIVIAL(debug) << "NetworkSever: StartServer";
  std::thread threadServerRunning(&ClientWorker::RunClientWorker, &_queue);
  threadServerRunning.detach();

  startAccept();
  run();
}

void NetworkSever::run() {
  BOOST_LOG_TRIVIAL(debug) << "NetworkSever: run service";
  boost::system::error_code ec;
  _service.run(ec);
  if (ec) {
    BOOST_LOG_TRIVIAL(fatal) << "NetworkSever: error run service: " << ec.message().c_str();
    return;
  }
}

void NetworkSever::startAccept() {
  BOOST_LOG_TRIVIAL(debug) << "NetworkSever: startAccept";
  auto user = std::make_shared<UserSession>(_service);
  _acceptor.async_accept(user->Sock(),
                         boost::bind(&NetworkSever::onAccept, this, user,
                                     boost::asio::placeholders::error));
}

void NetworkSever::onAccept(const std::shared_ptr<UserSession> &user, const boost::system::error_code &ec) {
  BOOST_LOG_TRIVIAL(debug) << "NetworkSever: onAccept";

  if (ec) {
    BOOST_LOG_TRIVIAL(error) << "NetworkSever: error accept user" << ec.message().c_str();
    return;
  }
  _queue.PutConnection(user);
  startAccept();
}

void NetworkSever::PutResponce(
    const std::shared_ptr<std::pair<std::shared_ptr<UserSession>, boost::property_tree::ptree> > &response) {
  BOOST_LOG_TRIVIAL(debug) << "NetworkSever: PutResponce";
  _queue.GetResponseFromServer(response);
}

std::shared_ptr<std::pair<std::shared_ptr<UserSession>,
                          boost::property_tree::ptree> > NetworkSever::GetRequest() {
  BOOST_LOG_TRIVIAL(debug) << "NetworkSever: GetRequest";
  return _queue.SendRequestToServer();
}



