#include "../../Client/include/ClientNetwork.h"
#include <iostream>
#include "SerializerChunk.h"

namespace pt = boost::property_tree;

int main(int argc, char *argv[]) {

  std::vector<Chunk> chunkVector;
  for (int i = 1; i < 3; ++i) {
    auto chunk = Chunk{.userId = 1,
        .chunkId = i,
        .chunkSize = i * 10,
        .rHash = "rhash",
        .sHash = "shash"};

    chunk.data.push_back('d');
    chunk.data.push_back('a');
    chunk.data.push_back('t');
    chunk.data.push_back('a');

    chunkVector.push_back(chunk);
  }

  auto request = SerializerChunk(0, chunkVector);

    ClientNetwork client;
  client.Connect();

  client.SendJSON(request.GetJson());

  pt::ptree response = client.ReceiveJSON();

  std::stringstream ss;
  boost::property_tree::write_json(ss, response);
  std::cout << "Reply is: " << ss.str() << std::endl;

  exit(EXIT_SUCCESS);
}