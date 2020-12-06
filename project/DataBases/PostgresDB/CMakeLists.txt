include_directories(Base/include
                      MetaDataDB/include
                     UserDB/include)
ADD_LIBRARY(libMetDataDB STATIC Base/src/PostgreSQLDB.cpp MetaDataDB/src/MetaDataDB.cpp)
ADD_LIBRARY(libUserDB STATIC Base/src/PostgreSQLDB.cpp UserDB/src/UsersDB.cpp)

add_executable(PostgresDB main.cpp
        Base/src/PostgreSQLDB.cpp MetaDataDB/src/MetaDataDB.cpp UserDB/src/UsersDB.cpp)
target_include_directories(PostgresDB PRIVATE ${PostgreSQL_INCLUDE_DIRS})
