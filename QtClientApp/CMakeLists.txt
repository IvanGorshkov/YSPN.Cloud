cmake_minimum_required(VERSION 3.5)

project(Cloud LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(CMAKE_PREFIX_PATH "/Users/s.alekhin/Qt/5.15.2/clang_64/lib/cmake")
find_package(Qt5 COMPONENTS Widgets REQUIRED)

FIND_LIBRARY(App-dev REQUIRED)
INCLUDE_DIRECTORIES(../project/App/include)
INCLUDE_DIRECTORIES(../project/utils/include)
INCLUDE_DIRECTORIES(../project/DataBases/InternalDB/include)
INCLUDE_DIRECTORIES(../project/Network/Client/include)
INCLUDE_DIRECTORIES(../project/Client/Chunker/include)
INCLUDE_DIRECTORIES(../project/Client/Indexer/include)

if (APPLE)
    INCLUDE_DIRECTORIES(../project/Client/Watcher/Apple/include)
    INCLUDE_DIRECTORIES(../project/Client/Watcher/include)
else ()
    INCLUDE_DIRECTORIES(../project/Client/Watcher/include)
    INCLUDE_DIRECTORIES(../project/Client/Watcher/Linux/include)
endif ()

INCLUDE_DIRECTORIES(include)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/mainwindow.cpp
        include/mainwindow.h
        src/mainwindow.ui
        )

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets App-dev)
