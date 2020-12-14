/**
 * Project Untitled
 */


#ifndef _FILECHUNK_H
#define _FILECHUNK_H
#include "stdlib.h"
#include <array>
#include <string>

#define CHUNK_SIZE       4096

struct FileChunk {
    std::array<char, CHUNK_SIZE> data;
    size_t chunkSize;
    std::string RHash;
    std::string SHash;
};

#endif //_FILECHUNK_H