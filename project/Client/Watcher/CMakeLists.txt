cmake_minimum_required(VERSION 3.17)
project(Watcher)

set(CMAKE_CXX_STANDARD 14)

INCLUDE_DIRECTORIES(include)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../utils)

add_executable(Watcher Notifier.cpp Watcher.cpp)

