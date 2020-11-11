/**
 * Project Untitled
 */


#ifndef _FILECHUNK_H
#define _FILECHUNK_H
#include "stdlib.h"
#include <array>
#include <string>

struct FileChunk {
    std::array<char, 256> data;
    size_t chunkSize;
    std::string RHash;
    std::string SHash;
};

#endif //_FILECHUNK_H