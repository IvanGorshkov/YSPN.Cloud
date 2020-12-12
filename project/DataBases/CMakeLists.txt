include_directories(InternalDB/include)
ADD_LIBRARY(libinternalDB STATIC InternalDB/src/InternalDB.cpp)
#ADD_LIBRARY(libMongoDB STATIC MongoDB/src/MongoDB.cpp)

find_package (SQLite3)
include_directories(${SQLite3_INCLUDE_DIRS})

find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(InternalDB InternalDB/src/main.cpp
        InternalDB/src/InternalDB.cpp)

target_link_libraries (InternalDB ${SQLite3_LIBRARIES})
target_link_libraries(InternalDB  ${Boost_LIBRARIES})

ADD_SUBDIRECTORY(PostgresDB)
ADD_SUBDIRECTORY(MongoDB)
ADD_SUBDIRECTORY(tests)
