/**
 * Project Untitled
 */


#ifndef _OUTFILE_H
#define _OUTFILE_H

#include "File.h"


class OutFile: public File {
public: 
    
void SetNextChunk();
private: 
    std::ofstream _file;
};

#endif //_OUTFILE_H