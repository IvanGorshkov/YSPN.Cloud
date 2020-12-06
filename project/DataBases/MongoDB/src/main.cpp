#include <iostream>
#include "MongoDB.h"


int main(int, char**)
{
  Chunks chunks;
  chunks.chunkSize = 23;
  chunks.idFile =1;
  chunks.rapidHash = "JR7udj)Hdjj@f;d";
  chunks.staticHash =  "JR7udjJR7udjJR7udjJR7udj";
  chunks.id = 9;
  std::vector<Chunks> hhh;
  hhh.push_back(chunks);
  auto& mongo_db = MongoDB::shared();
  mongo_db.InsertChunk(hhh);
  mongo_db.GetChunk();

}