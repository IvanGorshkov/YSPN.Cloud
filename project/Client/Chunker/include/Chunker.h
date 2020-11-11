/**
 * Project Untitled
 */


#ifndef _CHUNKER_H
#define _CHUNKER_H
#include "InFile.h"
#include "OutFile.h"
#include "ChunkData.h"

class Chunker {
public: 
    
int SentNewChunk();
    
int SentNewPosition();
    
InFile* GetFile();
    
/**
 * @param ChunkData
 */
void ChunkCompare(ChunkData data);
private: 
    std::string  _path;
    
std::string getCheckSum();
    
std::string getOldCheckSums();
    
ChunkData createChunk();
};

#endif //_CHUNKER_H