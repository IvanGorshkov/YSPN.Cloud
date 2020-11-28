CMAKE_MINIMUM_REQUIRED(VERSION 3.13.4)
PROJECT(YSNP.Cloud)

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wpedantic")

INCLUDE_DIRECTORIES(project/include)

ADD_EXECUTABLE(${PROJECT_NAME}
        project/src/main.cpp)

ENABLE_TESTING()
ADD_SUBDIRECTORY(project)
ADD_SUBDIRECTORY(project/Network)
ADD_SUBDIRECTORY(project/Application)
ADD_SUBDIRECTORY(project/testsNetwork)
ADD_SUBDIRECTORY(project/authorizationServer)
ADD_SUBDIRECTORY(project/storageServer)
ADD_SUBDIRECTORY(project/syncServer)
ADD_SUBDIRECTORY(project/utils)

