SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wpedantic")

INCLUDE_DIRECTORIES(include)

ADD_LIBRARY(network STATIC
        src/NetworkSever.cpp
        src/UserSession.cpp
        src/Connection.cpp
        src/ClientNetwork.cpp)