cmake_minimum_required(VERSION 3.17)
project(Chunker)

set(CMAKE_CXX_STANDARD 14)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../utils)
INCLUDE_DIRECTORIES(include)

add_executable(Chunker Chunker.cpp File.cpp InFile.cpp OutFile.cpp)