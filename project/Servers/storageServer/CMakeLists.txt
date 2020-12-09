CMAKE_MINIMUM_REQUIRED(VERSION 3.13.4)
PROJECT(StorageServer)

SET(CMAKE_CXX_STANDARD 17)
#SET(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -Wpedantic")

FIND_PACKAGE(Boost COMPONENTS log log_setup program_options REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

FIND_LIBRARY(Utils-dev REQUIRED)
FIND_LIBRARY(Server-dev REQUIRED)
FIND_LIBRARY(MongoDB-dev REQUIRED)

INCLUDE_DIRECTORIES(../server/include)
INCLUDE_DIRECTORIES(../../utils/include)
INCLUDE_DIRECTORIES(../../DataBases/MongoDB/include)
INCLUDE_DIRECTORIES(../../net)


INCLUDE_DIRECTORIES(include)

ADD_EXECUTABLE(${PROJECT_NAME}
        src/main.cpp
        src/StorageCommand.cpp
        src/StorageManager.cpp
        src/StorageServer.cpp
        )
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${Boost_LIBRARIES} Utils-dev Server-dev MongoDB-dev)

#ADD_EXECUTABLE(${PROJECT_NAME}-test
#        src/main_test.cpp)
#TARGET_LINK_LIBRARIES(${PROJECT_NAME}-test ${Boost_LIBRARIES})
