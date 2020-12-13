cmake_minimum_required(VERSION 3.17)
project(Chunker)

set(CMAKE_CXX_STANDARD 14)

ADD_SUBDIRECTORY(tests)

INCLUDE_DIRECTORIES(../utils)
INCLUDE_DIRECTORIES(include)

#add_executable(Chunker Chunker.cpp ../utils/File.cpp)
ADD_LIBRARY(libChunker STATIC
        include/Chunker.h
        Chunker.cpp
        ../utils/FileChunk.h
        ../utils/File.cpp
        ../utils/File.h)