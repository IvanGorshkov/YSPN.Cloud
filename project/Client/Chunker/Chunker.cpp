#include "Chunker.h"

Chunker::Chunker(File file)
    : _file(file) {}

int Chunker::SentNewChunk() {
  return 0;
}

int Chunker::SentNewPosition() {
  return 0;
}

void Chunker::ChunkCompare(Chunk data) {
  return;
}

std::vector<Chunk> Chunker::UpdateChunkFile(const std::vector<Chunk>& old_chunks) {
  std::ifstream fileStream(std::move(_file.Read()));
  size_t size = fileStream.seekg(0, std::ios::end).tellg();
  fileStream.seekg(0,std::ios::beg);
  std::vector<Chunk> chunks;

  if (fileStream.is_open()) {
    while (!fileStream.eof()) {
      std::array<char, CHUNK_SIZE> data{};

//      fileStream.read(data.data(), CHUNK_SIZE);
//      Chunk chunk{
//          .chunkSize = static_cast<int>(fileStream.gcount()),
//          .data = data,
//      };
//      getRHash(chunk);
//      getSHash(chunk);
//      chunks.push_back(std::move(chunk));
    }
  }
  fileStream.close();
  return chunks;
}

void Chunker::getSHash(Chunk &chunk) {
  MD5_CTX ctx;
  MD5_Init(&ctx);

  MD5_Update(&ctx, chunk.data.data(), chunk.chunkSize);

  unsigned char digest[MD5_DIGEST_LENGTH] = {};
  MD5_Final(digest, &ctx);

  std::stringstream result;
  for (unsigned char i : digest)
    result << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(i);

  chunk.sHash = result.str();
}

void Chunker::getRHash(Chunk &chunk) {
  uint32_t digest = adler32(0, reinterpret_cast<const Bytef *>(chunk.data.data()), chunk.chunkSize);

  std::stringstream result;
  result << std::hex << static_cast<int>(digest);
  chunk.rHash = result.str();
}

std::string Chunker::getOldCheckSums() {
  return "";
}

std::vector<Chunk> Chunker::ChunkFile() {
  int a = 0;
  std::ifstream fileStream(std::move(_file.Read()));
  size_t size = fileStream.seekg(0, std::ios::end).tellg();
  fileStream.seekg(0,std::ios::beg);
  std::vector<Chunk> chunks;
  if (fileStream.is_open()) {
    while (!fileStream.eof()) {
      std::array<char, CHUNK_SIZE> data{};
      fileStream.read(data.data(), CHUNK_SIZE);
      Chunk chunk{
          .chunkSize = static_cast<int>(fileStream.gcount()),
      };
      std::copy(data.begin(), std::find(data.begin(), data.end(), '\0'), std::back_inserter(chunk.data));
      getRHash(chunk);
      getSHash(chunk);
      chunks.push_back(std::move(chunk));

    }
  }
  fileStream.close();
  return chunks;
}

void Chunker::MergeFile(std::vector<Chunk> chunks) {
  // TODO: для беспорядка принимать мапу, где первое поле order
  std::ofstream outputFile(std::move(_file.Write()));

  if (outputFile.is_open()) {
    for (auto chunk : chunks) {
      outputFile.write(chunk.data.data(), chunk.chunkSize);
    }
  }
  outputFile.close();
}