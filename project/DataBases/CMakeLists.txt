find_package(PostgreSQL REQUIRED)

include_directories(InternalDB/include
        PostgresDB/Base/include
        PostgresDB/UserDB/include
        PostgresDB/MetaDataDB/include
        MongoDB/include)
ADD_LIBRARY(libinternalDB STATIC InternalDB/src/InternalDB.cpp)
ADD_LIBRARY(libMetDataDB STATIC PostgresDB/Base/src/PostgreSQLDB.cpp PostgresDB/MetaDataDB/src/MetaDataDB.cpp)
ADD_LIBRARY(libUserDB STATIC PostgresDB/Base/src/PostgreSQLDB.cpp PostgresDB/UserDB/src/UsersDB.cpp)
ADD_LIBRARY(libMongoDB STATIC MongoDB/src/MongoDB.cpp)

find_package (SQLite3)
include_directories(${SQLite3_INCLUDE_DIRS})
find_package(Boost)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(InternalDB InternalDB/src/main.cpp
        InternalDB/src/InternalDB.cpp)
add_executable(MongoDB MongoDB/src/main.cpp
        MongoDB/src/MongoDB.cpp)

target_link_libraries (InternalDB ${SQLite3_LIBRARIES})
target_link_libraries(InternalDB  ${Boost_LIBRARIES})

ADD_SUBDIRECTORY(tests)
add_executable(PostgresDB PostgresDB/main.cpp
        PostgresDB/Base/src/PostgreSQLDB.cpp PostgresDB/MetaDataDB/src/MetaDataDB.cpp PostgresDB/UserDB/src/UsersDB.cpp)
target_include_directories(PostgresDB PRIVATE ${PostgreSQL_INCLUDE_DIRS})
target_link_libraries(PostgresDB PRIVATE ${PostgreSQL_LIBRARIES})
