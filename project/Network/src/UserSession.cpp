#include "UserSession.h"

UserSession::UserSession(boost::asio::io_service &io)
        : _socket(io) {}

boost::property_tree::ptree UserSession::GetRequest() {
    BOOST_LOG_TRIVIAL(debug) << "UserSession: start get request";
    receiveRequest();
    return _jsonReceive;
}

void UserSession::SendResponse(const boost::property_tree::ptree &jsonSend) {
    BOOST_LOG_TRIVIAL(debug) << "UserSession: start send response";
    receiveResponse(jsonSend);
}

boost::asio::ip::tcp::socket &UserSession::Sock() {
    BOOST_LOG_TRIVIAL(debug) << "UserSession: return user socket";
    return _socket;
}

void UserSession::receiveRequest() {
    BOOST_LOG_TRIVIAL(debug) << "UserSession: receive request";
    int maxLength = 1024;
    boost::system::error_code ec;
    _socket.read_some(boost::asio::buffer(_readBuf, maxLength), ec);
    if (ec) {
        BOOST_LOG_TRIVIAL(error) << "UserSession: error read from user" << ec.message();
        _socket.async_write_some(
                boost::asio::buffer(ec.message()),
                [self = shared_from_this()](const boost::system::error_code &,
                                            std::size_t) -> void {
                    self->receiveRequest();
                });
        return;
    }
    std::stringstream ss(_readBuf);
    try {
        boost::property_tree::read_json(ss, _jsonReceive);
    } catch (boost::property_tree::ptree_error &e) {
        BOOST_LOG_TRIVIAL(error) << "UserSession: error write to ptree: " << e.what();
        return;
    }
}

void UserSession::receiveResponse(const boost::property_tree::ptree &jsonSend) {
    BOOST_LOG_TRIVIAL(error) << "UserSession: send response";
    std::stringstream ss;
    try {
        boost::property_tree::json_parser::write_json(ss, jsonSend, false);
    } catch (boost::property_tree::ptree_error &e) {
        BOOST_LOG_TRIVIAL(error) << "UserSession: error read from ptree: " << e.what();
        return;
    }
    char sendBuf[1024];
    strncpy(sendBuf, ss.str().c_str(), ss.str().length());
    sendBuf[ss.str().length() - 1] = 0;
    try {
        _socket.async_write_some(
                boost::asio::buffer(sendBuf),
                [self = shared_from_this()](const boost::system::error_code &,
                                            std::size_t) -> void {
                });
    } catch (boost::system::error_code &ec) {
        BOOST_LOG_TRIVIAL(error) << "UserSession: error send response to user: " << ec.message();
        return;
    }
}
