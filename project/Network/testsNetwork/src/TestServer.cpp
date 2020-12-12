#include "gtest_utils.h"

using namespace std;

TEST(NetworkSendReceive, work){
    std::cout << "main thread is " << pthread_self() << std::endl;

    NetworkSever server;
    server.StartServer();

}