CMAKE_MINIMUM_REQUIRED(VERSION 3.13.4)
PROJECT(Utils)

FIND_PACKAGE(Boost COMPONENTS log REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

SET(CMAKE_CXX_STANDARD 17)
#SET(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -Wpedantic")

INCLUDE_DIRECTORIES(include)

ADD_LIBRARY(${PROJECT_NAME}-dev STATIC
        src/SerializerChunk.cpp
        src/SerializerUserChunk.cpp
        src/SerializerAnswer.cpp
        src/SerializerUserDate.cpp
        src/SerializerFileInfo.cpp
        )
TARGET_LINK_LIBRARIES(${PROJECT_NAME}-dev ${Boost_LIBRARIES})

ENABLE_TESTING()
ADD_SUBDIRECTORY(test)