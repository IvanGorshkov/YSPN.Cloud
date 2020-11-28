#ifndef _USERSESSION_H
#define _USERSESSION_H

#include "Connection.h"
#include <queue>

class UserSession {
public:

    void GetRequest();

    void PutResponse();

    void ReceiveMeta();

    void SendMeta();

private:
    Connection _connection;
    std::queue<JSON> _request;
    std::queue<JSON> _response;

    void receiveRequest();

    void sendResponse();
};

#endif //_USERSESSION_H