SET(CMAKE_CXX_STANDARD 17)
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wpedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++17 -Wall")
FIND_PACKAGE(Boost COMPONENTS log REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
INCLUDE_DIRECTORIES(include)

ADD_LIBRARY(Network-dev SHARED
        src/NetworkSever.cpp
        src/UserSession.cpp
        src/ClientWorker.cpp
        )
TARGET_LINK_LIBRARIES(Network-dev ${Boost_LIBRARIES})

ADD_EXECUTABLE(testServer
        src/NetworkSever.cpp
        src/UserSession.cpp
        src/ClientWorker.cpp
        src/main.cpp)

ADD_EXECUTABLE(testClient
        src/test_client.cpp
        src/Client.cpp)
ADD_SUBDIRECTORY(testsNetwork)


TARGET_LINK_LIBRARIES(testServer ${Boost_LIBRARIES})
TARGET_LINK_LIBRARIES(testClient ${Boost_LIBRARIES})
