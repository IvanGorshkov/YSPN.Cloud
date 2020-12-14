
#include "Chunker.h"

Chunker::Chunker(File file)
: _file(file)
{}

int Chunker::SentNewChunk() {
    return 0;
}


int Chunker::SentNewPosition() {
    return 0;
}


File* Chunker::GetFile() {

    return nullptr;
}


void Chunker::ChunkCompare(FileChunk data) {
    return;
}


void Chunker::getSHash(FileChunk& chunk) {
    MD5_CTX ctx;
    MD5_Init(&ctx);

    MD5_Update(&ctx, chunk.data.data(), chunk.chunkSize);

    unsigned char digest[MD5_DIGEST_LENGTH] = {};
    MD5_Final(digest, &ctx);

    std::stringstream result;
    for(unsigned char i : digest)
        result << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(i);

    chunk.SHash = result.str();
}

void Chunker::getRHash(FileChunk& chunk) {
    uint32_t digest = adler32(0, reinterpret_cast<const Bytef *>(chunk.data.data()), chunk.chunkSize);

    std::stringstream result;
    result << std::hex << static_cast<int>(digest);
    chunk.RHash = result.str();
}


std::string Chunker::getOldCheckSums() {
    return "";
}

std::vector<FileChunk> Chunker::ChunkFile() {
    std::ifstream fileStream(std::move(_file.Read()));
    //size_t size = fileStream.seekg(0, std::ios::end).tellg();
    std::vector<FileChunk> chunks;

    if(fileStream.is_open()){
        while(!fileStream.eof()){
            std::array<char, CHUNK_SIZE> data{};
            fileStream.read(data.data(), 7);
            FileChunk chunk{ data, static_cast<size_t>(fileStream.gcount())};
            getRHash(chunk);
            getSHash(chunk);
            chunks.push_back(std::move(chunk));
        }
    }
    fileStream.close();
    return chunks;
}

void Chunker::MergeFile(std::vector<FileChunk> chunks) {
    std::ofstream outputFile(std::move(_file.Write()));

    if(outputFile.is_open()){
        for(auto chunk : chunks){
          outputFile.write(chunk.data.data(), chunk.chunkSize);
        }
    }
    outputFile.close();
}