
#pragma once

#include "File.h"
#include <openssl/md5.h>
#include <iomanip>
#include "zlib.h"
#include <memory>




class Chunker {
public: 
    explicit Chunker(File);
int SentNewChunk();
    
int SentNewPosition();
    
File* GetFile();

std::vector<FileChunk> ChunkFile();
void MergeFile(std::vector<FileChunk>);

void ChunkCompare(FileChunk data);
private: 
    File  _file;
    
std::string getSHashSum();
    
std::string getOldCheckSums();


};