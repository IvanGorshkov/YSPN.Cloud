cmake_minimum_required(VERSION 3.17)
project(Indexer)
SET(CMAKE_CXX_STANDARD 17)
find_package(Boost 1.54.0 COMPONENTS system filesystem REQUIRED)

set(CMAKE_CXX_STANDARD 14)

INCLUDE_DIRECTORIES(../../utils/include/structs)
INCLUDE_DIRECTORIES(../utils)
INCLUDE_DIRECTORIES(include)

add_executable(Indexer Indexer.cpp)

ADD_LIBRARY(libIndexer STATIC
        include/Indexer.h
        Indexer.cpp
        )