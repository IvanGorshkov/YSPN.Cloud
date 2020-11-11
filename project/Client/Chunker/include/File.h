/**
 * Project Untitled
 */


#ifndef _FILE_H
#define _FILE_H
#include "stdlib.h"
#include <vector>
#include <string>
#include "FileChunk.h"
#include <fstream>

class File {
public: 


protected: 
    std::string path;
    std::vector<FileChunk> Chunks;
    
void GetHashInternal();
private: 
    size_t _chunksCount;
};

#endif //_FILE_H