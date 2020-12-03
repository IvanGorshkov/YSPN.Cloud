CMAKE_MINIMUM_REQUIRED(VERSION 3.13.4)
PROJECT(StorageServer)

FIND_PACKAGE(Boost COMPONENTS log log_setup REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

SET(CMAKE_CXX_STANDARD 17)
#SET(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -Wpedantic")

INCLUDE_DIRECTORIES(include)

LINK_DIRECTORIES(../../cmake-build-debug)
INCLUDE_DIRECTORIES(../utils/include)

ADD_EXECUTABLE(${PROJECT_NAME}
        src/main.cpp
        src/Config.cpp
        src/StorageServer.cpp
        src/Worker.cpp
#        src/Commands.cpp
        )

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${Boost_LIBRARIES} Utils-dev)

#ADD_LIBRARY(${PROJECT_NAME}-dev src/StorageServer.cpp)

#ENABLE_TESTING()
#ADD_SUBDIRECTORY(tests)

#ADD_EXECUTABLE(ctest
#        ../utils/src/SerializerAnswer.cpp
#        src/test.cpp)
#
#TARGET_LINK_LIBRARIES(ctest ${Boost_LIBRARIES})
