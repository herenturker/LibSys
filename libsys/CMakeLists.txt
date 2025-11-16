cmake_minimum_required(VERSION 3.16)
project(LibSys LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.9.2/mingw_64")

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

qt_standard_project_setup()

file(GLOB_RECURSE SOURCES
    src/*.cpp
)

file(GLOB_RECURSE HEADERS
    src/headers/*.h
)

set(RESOURCES
    icons/resources.qrc
)

qt_add_executable(LibSys WIN32
    ${SOURCES}
    ${HEADERS}
    ${RESOURCES}
)

target_link_libraries(LibSys PRIVATE
    Qt6::Widgets
    Qt6::Sql
)
