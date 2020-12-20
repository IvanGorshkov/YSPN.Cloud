cmake_minimum_required(VERSION 3.16.5)
project(Watcher)

find_package(Boost 1.54.0 COMPONENTS system filesystem REQUIRED)

set(CMAKE_CXX_STANDARD 14)

ADD_SUBDIRECTORY(tests)

INCLUDE_DIRECTORIES(include)
INCLUDE_DIRECTORIES(../utils)

#add_executable(Watcher Watcher.cpp)

ADD_LIBRARY(libWatcher STATIC
        include/FileSystemEvent.h
        include/Watcher.h
        Watcher.cpp
        FileSystemEvent.cpp
        ../utils/Notification.h)

