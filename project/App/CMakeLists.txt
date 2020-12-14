CMAKE_MINIMUM_REQUIRED(VERSION 3.13.4)
PROJECT(App)

SET(CMAKE_CXX_STANDARD 17)
#SET(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -Wpedantic")

FIND_LIBRARY(InternalDB-dev REQUIRED)
INCLUDE_DIRECTORIES(../DataBases/InternalDB/include)

FIND_PACKAGE(Boost COMPONENTS log REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

FIND_LIBRARY(ClientNetwork-dev REQUIRED)
INCLUDE_DIRECTORIES(../Network/Client/include)

FIND_LIBRARY(Utils-dev REQUIRED)
INCLUDE_DIRECTORIES(../utils/include)

INCLUDE_DIRECTORIES(include)

ADD_LIBRARY(${PROJECT_NAME}-dev
        src/App.cpp
        src/Worker.cpp
        src/Commands.cpp
        src/ClientConfig.cpp
        )
TARGET_LINK_LIBRARIES(${PROJECT_NAME}-dev ${Boost_LIBRARIES} InternalDB-dev ClientNetwork-dev Utils-dev)
