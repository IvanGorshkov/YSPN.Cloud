#include <iostream>
#include "SerializerChunk.h"

namespace pt = boost::property_tree;

int main() {
  pt::ptree root;
  std::vector<Chunk> vector;
  vector.reserve(500);
  for (int i = 0; i < 500; ++i) {
    vector.emplace_back(i, i, "sHash", "rHash", "data");
  }
  SerializerChunk serializer(vector);
  pt::write_json("1.json", serializer.GetJson());

//  pt::read_json("2.json", root);
//  SerializerChunk serializer(root);
//  auto vec = serializer.GetChunk();

  return 0;
}