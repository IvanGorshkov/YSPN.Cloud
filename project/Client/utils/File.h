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
    File(std::string);
    ~File();
    std::ifstream Read();
    std::ofstream Write();

private:
    std::string _path;
};

#endif //_FILE_H