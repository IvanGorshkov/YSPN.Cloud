CMAKE_MINIMUM_REQUIRED(VERSION 3.13.4)
PROJECT(SyncServer)

SET(CMAKE_CXX_STANDARD 17)
#SET(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -Wpedantic")

FIND_PACKAGE(Boost COMPONENTS log program_options REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

FIND_LIBRARY(Utils-dev REQUIRED)
INCLUDE_DIRECTORIES(../../utils/include)

FIND_LIBRARY(Server-dev REQUIRED)
INCLUDE_DIRECTORIES(../server/include)

FIND_LIBRARY(Network-dev REQUIRED)
INCLUDE_DIRECTORIES(../../Network/Server/include)


INCLUDE_DIRECTORIES(include)

ADD_EXECUTABLE(${PROJECT_NAME}
        src/main.cpp
        src/SyncCommands.cpp
        src/SyncManager.cpp
        src/SyncServer.cpp
        src/TestPostgra.cpp
        )
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${Boost_LIBRARIES} Utils-dev Server-dev )
