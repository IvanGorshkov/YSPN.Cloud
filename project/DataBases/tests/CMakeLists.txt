FIND_PACKAGE(GTest REQUIRED)
FIND_PACKAGE(Threads REQUIRED)

INCLUDE_DIRECTORIES("${GTEST_INCLUDE_DIRS}")

PROJECT(UtilsTests)
SET(CMAKE_CXX_STANDARD 17)

INCLUDE_DIRECTORIES(include)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

ADD_EXECUTABLE(${PROJECT_NAME}
        src/main.cpp
        src/test_internaldb.cpp
        src/test_userdb.cpp
        src/test_mongodb.cpp
        src/test_metadb.cpp)

TARGET_LINK_LIBRARIES(${PROJECT_NAME}  ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} libinternalDB libUserDB libMongoDB libMetDataDB -lgmock)

ADD_TEST(NAME Tests COMMAND ${PROJECT_NAME})