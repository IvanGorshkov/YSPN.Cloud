#include "ClientNetwork.h"

ClientNetwork::ClientNetwork()
    : _socket(_service) {
  BOOST_LOG_TRIVIAL(debug) << "ClientNetwork: create ClientNetwork";
}

void ClientNetwork::SendJSON(const pt::ptree &jsonRequest) {
  BOOST_LOG_TRIVIAL(debug) << "ClientNetwork: SendJSON";
  std::stringstream ss;
  try {
    pt::json_parser::write_json(ss, jsonRequest, false);
  } catch (pt::ptree_error &ec) {
    BOOST_LOG_TRIVIAL(error) << "ClientNetwork: error read JSON, while sending:" << ec.what();
    throw SendToServerParseJsonException(ec.what());
  }
  char sendBuf[1024];

  size_t request_length = ss.str().length();
  strncpy(sendBuf, ss.str().c_str(), request_length);
  sendBuf[request_length - 1] = 0;

  boost::system::error_code ec;
  boost::asio::write(_socket, boost::asio::buffer(sendBuf, request_length), ec);
  if (ec) {
    BOOST_LOG_TRIVIAL(error) << "ClientNetwork: error send to server:" << ec.message().c_str();
    throw SendToServerException(ec.message());
  }
}

pt::ptree ClientNetwork::ReceiveJSON() {
  BOOST_LOG_TRIVIAL(debug) << "ClientNetwork: ReceiveJSON";
  auto buf = std::make_shared<std::vector<char>>(1024);
  boost::system::error_code ec;
  boost::asio::read(_socket, boost::asio::buffer(*buf), ec);
  if (ec) {
    BOOST_LOG_TRIVIAL(error) << "ClientNetwork: error read from server:" << ec.message().c_str();
    throw ReadFromServerException(ec.message());
  }

  std::string response = buf->data();
  std::stringstream ss(response);
  pt::ptree jsonResponse;
  try {
    pt::read_json(ss, jsonResponse);
  } catch (pt::ptree_error &ec) {
    BOOST_LOG_TRIVIAL(error) << "ClientNetwork: error write JSON to ptree, while receiving: " << ec.what();
    throw ReadFromServerParseJsonException(ec.what());
  }
  return jsonResponse;
}

void ClientNetwork::Connect(const std::string &localhost, int port) {
  BOOST_LOG_TRIVIAL(debug) << "ClientNetwork: connect to server: " << localhost << " " << port;
  boost::asio::ip::tcp::resolver resolver(_service);
  std::string sport = std::to_string(port);

  boost::system::error_code ec;
  boost::asio::connect(_socket, resolver.resolve({localhost, sport}), ec);
  if (ec) {
    BOOST_LOG_TRIVIAL(error) << "ClientNetwork: error connect to server: " << ec.message().c_str();
    throw ConnectServerException(ec.message());
  }
}

void ClientNetwork::Disconnect() {
  _socket.close();
}

ClientNetwork::~ClientNetwork() = default;
