cmake_minimum_required(VERSION 3.17)
project(Chunker)

set(CMAKE_CXX_STANDARD 14)

find_package(ZLIB REQUIRED)
find_package(OpenSSL REQUIRED)
if (OPENSSL_FOUND)
    if (APPLE)
        include_directories(/usr/local/Cellar/openssl@1.1/1.1.1i/include)
        list(APPEND LIB_LIST /usr/local/Cellar/openssl@1.1/1.1.1i/lib/libssl.dylib)
        list(APPEND LIB_LIST /usr/local/Cellar/openssl@1.1/1.1.1i/lib/libcrypto.dylib)
        message(STATUS "OpenSSL Version: ${OPENSSL_VERSION} ${OPENSSL_INCLUDE_DIR} ${OPENSSL_LIBRARIES}")
    else ()
        include_directories(${OPENSSL_INCLUDE_DIR})
        list(APPEND LIB_LIST ${OPENSSL_LIBRARIES})
        message(STATUS "OpenSSL Version: ${OPENSSL_VERSION} ${OPENSSL_INCLUDE_DIR} ${OPENSSL_LIBRARIES}")
    endif ()
endif ()

INCLUDE_DIRECTORIES(../../utils/include)
INCLUDE_DIRECTORIES(include)

ADD_LIBRARY(${PROJECT_NAME}-dev SHARED
        include/Chunker.h
        src/Chunker.cpp
        src/File.cpp
        include/File.h
        )
if (APPLE)
    target_link_libraries(${PROJECT_NAME}-dev Crypto ZLIB::ZLIB)
else ()
    target_link_libraries(${PROJECT_NAME}-dev OpenSSL::Crypto ZLIB::ZLIB)
endif ()

ADD_SUBDIRECTORY(tests)