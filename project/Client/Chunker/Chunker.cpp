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
  Chunk new_chunk;
  if (fileStream.is_open()) {
    std::vector<char> data(CHUNK_SIZE);
    fileStream.read(data.data(), CHUNK_SIZE);
    while (!fileStream.eof()) {
      std::string chunksRHash = getRHash(data.data(), static_cast<int>(fileStream.gcount()));
      auto match = std::find_if(old_chunks.begin(), old_chunks.end(), [&chunksRHash](const Chunk& old_chunk){
        return old_chunk.rHash == chunksRHash;
      });
      if(match != old_chunks.end()){
        chunks.push_back(*match);
        fileStream.read(data.data(), CHUNK_SIZE);
        // smothing more
      } else {
        new_chunk.chunkSize += static_cast<int>(fileStream.gcount()); // check size
        std::copy(data.begin(), data.begin() + CHUNK_MOVE_SIZE, std::back_inserter(new_chunk.data));
        data.erase(data.begin(), data.begin()+CHUNK_MOVE_SIZE);
        fileStream.read(data.data() + 3072, CHUNK_MOVE_SIZE);
      }
//      getSHash(chunk);
//      chunks.push_back(std::move(chunk));
    }
  }
  fileStream.close();
  return chunks;
}

std::string Chunker::getSHash(char *data, int chunkSize) {
  MD5_CTX ctx;
  MD5_Init(&ctx);

  MD5_Update(&ctx, data, chunkSize);

  unsigned char digest[MD5_DIGEST_LENGTH] = {};
  MD5_Final(digest, &ctx);

  std::stringstream result;
  for (unsigned char i : digest)
    result << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(i);

  return result.str();
}

std::string Chunker::getRHash(char *data, int chunkSize) {
  uint32_t digest = adler32(0, reinterpret_cast<const Bytef *>(data), chunkSize);

  std::stringstream result;
  result << std::hex << static_cast<int>(digest);
  return result.str();
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
      std::vector<char> data(CHUNK_SIZE);
      fileStream.read(data.data(), CHUNK_SIZE);
      // createchunk
      Chunk chunk{
          .chunkSize = static_cast<int>(fileStream.gcount()),
      };
      std::copy(data.begin(), std::find(data.begin(), data.end(), '\0'), std::back_inserter(chunk.data));
      chunk.rHash = getRHash(chunk.data.data(), chunk.chunkSize);
      chunk.sHash = getSHash(chunk.data.data(), chunk.chunkSize);

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