#ifndef _CONNECTION_H
#define _CONNECTION_H

class Connection {
public:

    virtual void send() = 0;

    void Receive();

    void Send();

private:
   // boost::asio::ip::tcp::socket  _socket;

    void serialize();

    void deserialize();

    void sendJSON();

    void receiveJSON();
};

#endif //_CONNECTION_H