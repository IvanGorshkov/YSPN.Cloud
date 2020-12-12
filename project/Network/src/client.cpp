#include "Client.h"
#include <iostream>

int main(int argc, char* argv[]) {
    Client client;
    client.Connect();

    std::string message = R"({"root":{"values": [1, 2, 3, 4, 5 ]}})";
    std::cout << "Send message: " << message << std::endl;
    boost::property_tree::ptree request;
    std::stringstream ss(message);

    boost::property_tree::read_json(ss, request);

    client.SendJSON(request);

    boost::property_tree::ptree response = client.ReceiveJSON();
    boost::property_tree::write_json(ss, response);
    std::cout << "Reply is: "<< ss.str() << std::endl;
    exit(EXIT_SUCCESS);
}