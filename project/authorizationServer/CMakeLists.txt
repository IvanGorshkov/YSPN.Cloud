CMAKE_MINIMUM_REQUIRED(VERSION 3.13.4)
PROJECT(AuthorizationServer)

SET(CMAKE_CXX_STANDARD 17)
#SET(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -Wpedantic")

INCLUDE_DIRECTORIES(include)

ENABLE_TESTING()
ADD_SUBDIRECTORY(tests)

ADD_EXECUTABLE(${PROJECT_NAME} src/main.cpp src/AuthorizationServer.cpp)
ADD_LIBRARY(${PROJECT_NAME}-dev src/AuthorizationServer.cpp)