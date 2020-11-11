#ifndef _CLIENTNETWORK_H
#define _CLIENTNETWORK_H

#include "Connection.h"
#include <queue>

class ClientNetwork {
public:

    void RunClientNetwork();

    void GetResponse();

    void PutRequest();

private:
    Connection _connection;
    std::queue<JSON> _request;
    std::queue<JSON> _response;
};

#endif //_CLIENTNETWORK_H