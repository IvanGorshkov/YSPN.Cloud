FIND_PACKAGE(GTest REQUIRED)
FIND_PACKAGE(Threads REQUIRED)
INCLUDE_DIRECTORIES("${GTEST_INCLUDE_DIRS}")

PROJECT(tests_gtest)
SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wpedantic")

#add_library(study_std_lib STATIC )

include_directories(${GTEST_INCLUDE_DIRS})

add_executable(tests_gtest main.cpp)

target_link_libraries(tests_gtest ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} -lgmock)

ADD_TEST(NAME Test COMMAND ${PROJECT_NAME})