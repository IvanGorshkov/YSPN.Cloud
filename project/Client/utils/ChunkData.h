/**
 * Project Untitled
 */


#ifndef _CHUNKDATA_H
#define _CHUNKDATA_H
#include "stdlib.h"
#include <string>

struct ChunkData {
    std::string RHash;
    std::string SHash;
    size_t size;
};

#endif //_CHUNKDATA_H