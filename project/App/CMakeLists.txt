CMAKE_MINIMUM_REQUIRED(VERSION 3.13.4)
PROJECT(App)

SET(CMAKE_CXX_STANDARD 17)
#SET(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -Wpedantic")

FIND_LIBRARY(InternalDB-dev REQUIRED)
INCLUDE_DIRECTORIES(../DataBases/InternalDB/include)

FIND_PACKAGE(Boost COMPONENTS log REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

INCLUDE_DIRECTORIES(include)

ADD_LIBRARY(${PROJECT_NAME}-dev
        src/App.cpp
        src/Worker.cpp
        src/Commands.cpp
        src/ClientConfig.cpp
        )
TARGET_LINK_LIBRARIES(${PROJECT_NAME}-dev InternalDB-dev ${Boost_LIBRARIES})
