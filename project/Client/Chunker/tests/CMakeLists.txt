FIND_PACKAGE(GTest REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)
INCLUDE_DIRECTORIES("${GTEST_INCLUDE_DIRS}")

PROJECT(tests_gtest)
SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wpedantic")

include_directories(${GTEST_INCLUDE_DIRS})
include_directories(include)
include_directories(../include)
include_directories(../../utils)

add_executable(tests_gtest main.cpp test_Chunker.cpp)

target_link_libraries(tests_gtest ${GTEST_BOTH_LIBRARIES}  ${CMAKE_THREAD_LIBS_INIT} libChunker OpenSSL::Crypto ZLIB::ZLIB)

ADD_TEST(NAME Test COMMAND ${PROJECT_NAME})