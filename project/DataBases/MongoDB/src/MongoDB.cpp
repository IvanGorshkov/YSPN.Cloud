#include "MongoDB.h"
#include <bsoncxx/types.hpp>
#include <boost/lexical_cast.hpp>
#include <iterator>

MongoDB::MongoDB() : _client(_uri) {
  _database = _client["mydb"];
}

MongoDB &MongoDB::shared() {
  static MongoDB shared;
  return shared;
}

MongoDB::~MongoDB() {
  close();
}

void MongoDB::createDB() {}

void MongoDB::InsertChunk(const std::vector<Chunk>& chunks) {
  std::vector<bsoncxx::document::value> inChunks;

  for (const auto &chunk : chunks) {
	bsoncxx::types::b_binary b_binary{bsoncxx::binary_sub_type::k_binary, uint32_t(chunk.data.size()),
									  (uint8_t *) chunk.data.data()};
	inChunks.push_back(
		document{} << "id_user" << chunk.userId
				   << "id_chunk" << chunk.chunkId
				   << "rapid_hash" << chunk.rHash
				   << "static_hash" << chunk.sHash
				   << "data" << b_binary << finalize
	);
  }
  _database["restaurants"].insert_many(inChunks);

}

const std::vector<Chunk> MongoDB::GetChunk(const std::vector<UserChunk>& userChunks) const{
  std::vector<Chunk> users;
  for (auto userChunk : userChunks) {
	Chunk chunk;
	bsoncxx::stdx::optional<bsoncxx::document::value> cursor =
		_database["restaurants"]
			.find_one(document{} << "id_user" << userChunk.userId << "id_chunk" << userChunk.chunkId << finalize);
	if (cursor) {
	  bsoncxx::document::value value = (*cursor);
	  bsoncxx::document::view view = value.view();
	  chunk.userId = view["id_user"].get_int32();
	  chunk.chunkId = view["id_chunk"].get_int32();
	  chunk.sHash = view["static_hash"].get_utf8().value.to_string();
	  chunk.rHash = view["rapid_hash"].get_utf8().value.to_string();
	  const unsigned char *data = view["data"].get_binary().bytes;
	  std::vector<uint8_t> myVector;
	  myVector.reserve(view["data"].get_binary().size);
	  std::copy(data, data + view["data"].get_binary().size, std::back_inserter(myVector));
	  std::string str(myVector.begin(), myVector.end());
	  chunk.data = str;
	  users.push_back(chunk);
	}
  }
  return users;
}

void MongoDB::connect() {}

void MongoDB::close() {}