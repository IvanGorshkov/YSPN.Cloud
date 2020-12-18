CMAKE_MINIMUM_REQUIRED(VERSION 3.13.4)
PROJECT(PostgreSQL)

SET(CMAKE_CXX_STANDARD 17)
#SET(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -Wpedantic")

FIND_PACKAGE(PostgreSQL REQUIRED)

FIND_PACKAGE(Boost REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

INCLUDE_DIRECTORIES(Base/include
        MetaDataDB/include
        UserDB/include
        ../../utils/include)

ADD_LIBRARY(MetDataDB-dev SHARED
        Base/src/PostgreSQLDB.cpp
        MetaDataDB/src/MetaDataDB.cpp
        )

# Add include directories to your target. PRIVATE is useful with multi-target projects - see documentation of target_include_directories for more info
target_include_directories(MetDataDB-dev PRIVATE ${PostgreSQL_INCLUDE_DIRS})

# Add libraries to link your target againts. Again, PRIVATE is important for multi-target projects
target_link_libraries(MetDataDB-dev PRIVATE ${PostgreSQL_LIBRARIES})

ADD_LIBRARY(UserDB SHARED
        Base/src/PostgreSQLDB.cpp
        UserDB/src/UsersDB.cpp
        )

ADD_EXECUTABLE(PostgresDB
        main.cpp
        Base/src/PostgreSQLDB.cpp
        MetaDataDB/src/MetaDataDB.cpp
        UserDB/src/UsersDB.cpp)

# Add include directories to your target. PRIVATE is useful with multi-target projects - see documentation of target_include_directories for more info
target_include_directories(PostgresDB PRIVATE ${PostgreSQL_INCLUDE_DIRS})

# Add libraries to link your target againts. Again, PRIVATE is important for multi-target projects
target_link_libraries(PostgresDB PRIVATE ${PostgreSQL_LIBRARIES})