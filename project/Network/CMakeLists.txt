SET(CMAKE_CXX_STANDARD 17)
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wpedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++17 -Wall -ggdb3 -pthread -lboost_system")
FIND_PACKAGE(Boost COMPONENTS log REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
INCLUDE_DIRECTORIES(include)

ADD_LIBRARY(network STATIC
        src/NetworkSever.cpp
        src/UserSession.cpp
        src/ClientNetwork.cpp
        src/main.cpp)

ADD_EXECUTABLE(testServer
        src/NetworkSever.cpp
        src/UserSession.cpp
        src/ClientNetwork.cpp
        src/main.cpp)

ADD_EXECUTABLE(testClient
        src/client.cpp
        src/Client.cpp)
ADD_SUBDIRECTORY(testsNetwork)

TARGET_LINK_LIBRARIES(network ${Boost_LIBRARIES})
TARGET_LINK_LIBRARIES(testServer ${Boost_LIBRARIES})
TARGET_LINK_LIBRARIES(testClient ${Boost_LIBRARIES})
