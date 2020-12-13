cmake_minimum_required(VERSION 3.17)
project(Indexer)

find_package(Boost 1.54.0 COMPONENTS system filesystem REQUIRED)

set(CMAKE_CXX_STANDARD 14)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../utils)
INCLUDE_DIRECTORIES(include)

add_executable(Indexer Indexer.cpp)

ADD_LIBRARY(libIndexer STATIC
        include/Indexer.h
        Indexer.cpp
        ../utils/File.h
        ../utils/File.cpp)