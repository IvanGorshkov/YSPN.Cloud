CMAKE_MINIMUM_REQUIRED(VERSION 3.13.4)
PROJECT(Indexer)

SET(CMAKE_CXX_STANDARD 17)
#SET(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -Wpedantic")

FIND_PACKAGE(Boost COMPONENTS system filesystem REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

INCLUDE_DIRECTORIES(../../utils/include/structs)
INCLUDE_DIRECTORIES(../utils)

FIND_LIBRARY(InternalDB-dev REQUIRED)
INCLUDE_DIRECTORIES(../../DataBases/InternalDB/include)

INCLUDE_DIRECTORIES(include)
ADD_LIBRARY(${PROJECT_NAME}-dev SHARED
        include/Indexer.h
        Indexer.cpp
        )
add_executable(Indexer Indexer.cpp)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${Boost_LIBRARIES})

