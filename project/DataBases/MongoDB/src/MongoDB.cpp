#include "MongoDB.h"

MongoDB::MongoDB():_client(_uri) {
  _database = _client["mydb"];
}

MongoDB& MongoDB::shared() {
  static MongoDB shared;
  return shared;
}


MongoDB::~MongoDB() {
  close();
}

void MongoDB::createDB() { }

void MongoDB::InsertChunk(std::vector<Chunks> chunks) {
  std::vector<bsoncxx::document::value> inChunks;
  std::vector<std::string> chars;
  chars.push_back("1234");
  bsoncxx::types::b_binary  b_binary {bsoncxx::binary_sub_type::k_binary, uint32_t(chars.size()),
									  (uint8_t *)chars.data()};

  for (const auto & chunk : chunks) {
	inChunks.push_back(
		document{} << "id_user" << 1
		<< "id_chunk" << chunk.id
		<< "rapid_hash" << chunk.rapidHash
		<< "static_hash"  << chunk.staticHash
		<< "data" << b_binary  << finalize
		);
	std::cout << chunk.id;
  }
  _database["restaurants"].insert_many(inChunks);

}

void MongoDB::GetChunk() {
  mongocxx::cursor cursor = _database["restaurants"].find({});
  std::vector<std::string> arr;
  for(auto doc : cursor) {

	std::cout << bsoncxx::to_json(doc) << "\n";
	arr.push_back(bsoncxx::to_json(doc));
  }

}


void MongoDB::connect() { }

void MongoDB::close() { }