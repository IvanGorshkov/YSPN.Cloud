FIND_PACKAGE(GTest REQUIRED CONFIG)
FIND_PACKAGE(Threads REQUIRED)

PROJECT(AuthorizationServerTests)
SET(CMAKE_CXX_STANDARD 17)
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wpedantic")

INCLUDE_DIRECTORIES(include)
ADD_EXECUTABLE(${PROJECT_NAME}
        src/main.cpp
        src/tests_AuthorizationServer.cpp)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} GTest::gtest GTest::gmock GTest::gmock_main ${CMAKE_THREAD_LIBS_INIT} AuthorizationServer-dev)

ADD_TEST(NAME Tests COMMAND ${PROJECT_NAME})
