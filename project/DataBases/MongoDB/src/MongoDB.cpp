#include "MongoDB.h"
#include <memory>
#include <boost/log/trivial.hpp>
#include "MongoExceptions.h"

MongoDB::MongoDB() : _client(_uri) {
  _database = _client["cloud"];
  BOOST_LOG_TRIVIAL(debug) << "MongoDB: Init DB";
}

MongoDB &MongoDB::shared() {
  static MongoDB shared;
  return shared;
}

MongoDB::~MongoDB() = default;

void MongoDB::InsertChunk(const std::vector<Chunk>& chunks) const {
  BOOST_LOG_TRIVIAL(debug) << "MongoDB: Insert Chunks";
  std::vector<bsoncxx::document::value> inChunks;

  for (const auto &chunk : chunks) {
	bsoncxx::stdx::optional<bsoncxx::document::value> cursor =
		_database["chunks"].find_one(
			document{} << "id_user" << chunk.userId
					   << "id_chunk" << chunk.chunkId << finalize
		);
	if (cursor) {
<<<<<<< HEAD
	  throw MongoExceptions("This user = " + std::to_string(chunk.userId) + " and this chunk id = " + std::to_string(chunk.chunkId) + " is already exist");
=======
	  throw MongoExceptions("This user = " + std::to_string(chunk.userId) + " and this chunk id = " + std::to_string(chunk.chunkId) + "is already exist");
>>>>>>> origin/ivan_dev
	}
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
  _database["chunks"].insert_many(inChunks);
}

std::vector<Chunk> MongoDB::GetChunk(const std::vector<UserChunk>& userChunks) const{
  BOOST_LOG_TRIVIAL(debug) << "MongoDB: Get Chunks";
  std::vector<Chunk> users;
  for (const auto &userChunk : userChunks) {
	Chunk chunk;
	bsoncxx::stdx::optional<bsoncxx::document::value> cursor =
		_database["chunks"].find_one(
			document{} << "id_user" << userChunk.userId
			<< "id_chunk" << userChunk.chunkId << finalize
			);
	if (cursor) {
	  bsoncxx::document::value value = (*cursor);
	  bsoncxx::document::view view = value.view();
	  chunk.userId = view["id_user"].get_int32();
	  chunk.chunkId = view["id_chunk"].get_int32();
	  chunk.sHash = view["static_hash"].get_utf8().value.to_string();
	  chunk.rHash = view["rapid_hash"].get_utf8().value.to_string();
	  const unsigned char* data = view["data"].get_binary().bytes;
	  std::vector<uint8_t> dataVector;
	  dataVector.reserve(view["data"].get_binary().size);
	  std::copy(data, data + view["data"].get_binary().size, std::back_inserter(dataVector));
	  std::string str(dataVector.begin(), dataVector.end());
	  chunk.data = str;
	  users.push_back(chunk);
	} else {
<<<<<<< HEAD
	  throw MongoExceptions("This user = " + std::to_string(userChunk.userId) + " and this chunk id = " + std::to_string(userChunk.chunkId) + " is not exist");
=======
	  throw MongoExceptions("This user = " + std::to_string(userChunk.userId) + " and this chunk id = " + std::to_string(userChunk.chunkId) + "is already exist");
>>>>>>> origin/ivan_dev
	}
  }
  return users;
}