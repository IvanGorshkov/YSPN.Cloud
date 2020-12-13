#include "File.h"

File::File(std::string path): _path(path) {}

std::ifstream File::Read() {
    std::ifstream fileStream;
    fileStream.open(_path.c_str(), std::ios::in | std::ios::binary);
    return fileStream;
}

std::ofstream File::Write() {
    std::ofstream fileStream;
    fileStream.open(_path.c_str(), std::ios::out | std::ios::binary);
    return fileStream;
}

File::~File() {}