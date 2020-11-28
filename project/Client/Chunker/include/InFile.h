/**
 * Project Untitled
 */


#ifndef _INFILE_H
#define _INFILE_H

#include "File.h"


class InFile: public File {
    
void GetNextChunk();
private: 
    std::ifstream _file;
};

#endif //_INFILE_H