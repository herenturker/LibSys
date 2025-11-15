cmake_minimum_required(VERSION 3.16)
project(libsys LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.9.2/mingw_64")

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql SerialPort)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

qt_standard_project_setup()

file(GLOB_RECURSE SOURCES
    libsys/src/*.cpp
)

file(GLOB_RECURSE HEADERS
    libsys/src/headers/*.h
)

set(RESOURCES
    icons/resources.qrc
)

qt_add_executable(libsys
    ${SOURCES}
    ${HEADERS}
    ${RESOURCES}
)

target_link_libraries(libsys PRIVATE
    Qt6::Widgets
    Qt6::Sql
    Qt6::SerialPort
)
