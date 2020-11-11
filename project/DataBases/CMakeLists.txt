include_directories(InternalDB/include
        PostgresDB/Base/include
        PostgresDB/UserDB/include
        PostgresDB/MetaDataDB/include
        MongoDB/include)
ADD_LIBRARY(libinternalDB STATIC InternalDB/src/InternalDB.cpp)
ADD_LIBRARY(libMetDataDB STATIC PostgresDB/Base/src/PostgreSQLDB.cpp
        PostgresDB/MetaDataDB/src/MetaDataDB.cpp)
ADD_LIBRARY(libUserDB STATIC PostgresDB/Base/src/PostgreSQLDB.cpp
        PostgresDB/UserDB/src/UsersDB.cpp)
ADD_LIBRARY(libMongoDB STATIC MongoDB/src/MongoDB.cpp)
