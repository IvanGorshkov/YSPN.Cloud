CMAKE_MINIMUM_REQUIRED(VERSION 3.13.4)
PROJECT(App)

SET(CMAKE_CXX_STANDARD 17)
#SET(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -Wpedantic")

INCLUDE_DIRECTORIES(include)

ADD_LIBRARY(${PROJECT_NAME}-dev
        src/App.cpp
        src/Worker.cpp
        src/Commands.cpp
        )
