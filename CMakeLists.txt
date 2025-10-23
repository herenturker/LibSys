cmake_minimum_required(VERSION 3.16)
project(libsys LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.9.2/mingw_64")

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

file(GLOB_RECURSE SOURCES
    src/*.cpp
    sqlite/*.c
)

add_executable(libsys ${SOURCES})

target_include_directories(libsys PRIVATE src/headers sqlite)
target_link_libraries(libsys PRIVATE Qt6::Widgets)
