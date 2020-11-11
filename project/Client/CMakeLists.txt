cmake_minimum_required(VERSION 3.17)
project(Client)

set(CMAKE_CXX_STANDARD 14)

ADD_SUBDIRECTORY(Watcher)
ADD_SUBDIRECTORY(Indexer)
ADD_SUBDIRECTORY(Chunker)
ADD_SUBDIRECTORY(tests)

add_executable(Client main.cpp)