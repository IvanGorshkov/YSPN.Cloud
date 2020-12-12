#include <Client.h>

Client::Client() : _socket(_service) {}

void Client::SendJSON(const boost::property_tree::ptree &jsonRequest) {
    BOOST_LOG_TRIVIAL(debug) << "Client: send JSON";
    std::stringstream ss;
    try {
        boost::property_tree::json_parser::write_json(ss, jsonRequest, false);
    } catch (boost::property_tree::ptree_error &ec) {
        BOOST_LOG_TRIVIAL(error) << "Client: error read ptree:" << ec.what();
        return;
    }
    char sendBuf[1024];

    size_t request_length = ss.str().length();
    strncpy(sendBuf, ss.str().c_str(), request_length);
    sendBuf[request_length - 1] = 0;

    boost::system::error_code ec;
    boost::asio::write(_socket, boost::asio::buffer(sendBuf, request_length), ec);
    if (ec) {
        BOOST_LOG_TRIVIAL(error) << "Client: error write to server:" << ec.message();
        return;
    }
}

boost::property_tree::ptree Client::ReceiveJSON() {
    BOOST_LOG_TRIVIAL(debug) << "Client: receive JSON";
    auto buf = std::make_shared<std::vector<char>>(1024);
    boost::system::error_code ec;
    boost::asio::read(_socket, boost::asio::buffer(*buf), ec);
    if (ec) {
        BOOST_LOG_TRIVIAL(error) << "Client: error read from server:" << ec.message();
        std::stringstream ss(R"({"error": "Error occured" })");
        boost::property_tree::ptree jsonError;
        boost::property_tree::read_json(ss, jsonError);
        return jsonError;
    }

    std::string response = buf->data();
    std::stringstream ss(response);
    boost::property_tree::ptree jsonResponse;
    try {
        boost::property_tree::read_json(ss, jsonResponse);
    } catch (boost::property_tree::ptree_error &ec) {
        BOOST_LOG_TRIVIAL(error) << "Client: error write ptree: " << ec.what();
        std::stringstream ss(R"({"error": "Error occured" })");
        boost::property_tree::ptree jsonError;
        boost::property_tree::read_json(ss, jsonError);
        return jsonError;
    }
    return jsonResponse;
}

void Client::Connect(const std::string &localhost, int port) {
    BOOST_LOG_TRIVIAL(debug) << "Client: connect to server: " << localhost << " " << port;
    boost::asio::ip::tcp::resolver resolver(_service);
    std::string sport = std::to_string(port);

    boost::system::error_code ec;
    boost::asio::connect(_socket, resolver.resolve({localhost, sport}), ec);
    if (ec){
        BOOST_LOG_TRIVIAL(error) << "Client: error connect to server:" << ec.message();
        return;
    }
}

Client::~Client() = default;

