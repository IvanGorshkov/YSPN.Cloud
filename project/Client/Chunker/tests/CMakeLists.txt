FIND_PACKAGE(GTest REQUIRED)
FIND_PACKAGE(Threads REQUIRED)
#find_package(
#        Boost 1.54.0
#        COMPONENTS unit_test_framework system filesystem
#        REQUIRED
#)

PROJECT(tests_gtest_client)
SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wpedantic")

find_package(OpenSSL REQUIRED)
if(OPENSSL_FOUND)
    if(APPLE)
        include_directories(/usr/local/Cellar/openssl@1.1/1.1.1i/include)
        list(APPEND LIB_LIST /usr/local/Cellar/openssl@1.1/1.1.1i/lib/libssl.dylib)
        list(APPEND LIB_LIST /usr/local/Cellar/openssl@1.1/1.1.1i/lib/libcrypto.dylib)
        message(STATUS "OpenSSL Version: ${OPENSSL_VERSION} ${OPENSSL_INCLUDE_DIR} ${OPENSSL_LIBRARIES}")
    else()
        include_directories(${OPENSSL_INCLUDE_DIR})
        list(APPEND LIB_LIST ${OPENSSL_LIBRARIES})
        message(STATUS "OpenSSL Version: ${OPENSSL_VERSION} ${OPENSSL_INCLUDE_DIR} ${OPENSSL_LIBRARIES}")
    endif()
endif()

INCLUDE_DIRECTORIES("${GTEST_INCLUDE_DIRS}")


include_directories(${GTEST_INCLUDE_DIRS})
#INCLUDE_DIRECTORIES(/usr/local/opt/openssl@1.1/include)
include_directories(include)
include_directories(../include)
include_directories(../../utils)

add_executable(tests_gtest_client main.cpp test_Chunker.cpp)

target_link_libraries(tests_gtest_client ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} libChunker Crypto)  #Boost::system
#        Boost::filesystem)

ADD_TEST(NAME Test COMMAND ${PROJECT_NAME})