
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ")
include_directories(Base/include
                      MetaDataDB/include
                     UserDB/include
                    ../../utils/include)
ADD_LIBRARY(libMetDataDB STATIC Base/src/PostgreSQLDB.cpp MetaDataDB/src/MetaDataDB.cpp)
ADD_LIBRARY(libUserDB STATIC Base/src/PostgreSQLDB.cpp UserDB/src/UsersDB.cpp)

add_executable(PostgresDB main.cpp
        Base/src/PostgreSQLDB.cpp MetaDataDB/src/MetaDataDB.cpp UserDB/src/UsersDB.cpp)
# This command attempts to find the library, REQUIRED argument is optional
find_package(PostgreSQL REQUIRED)

# Add include directories to your target. PRIVATE is useful with multi-target projects - see documentation of target_include_directories for more info
target_include_directories(PostgresDB PRIVATE ${PostgreSQL_INCLUDE_DIRS})

# Add libraries to link your target againts. Again, PRIVATE is important for multi-target projects
target_link_libraries(PostgresDB PRIVATE ${PostgreSQL_LIBRARIES})