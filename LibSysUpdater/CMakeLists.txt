cmake_minimum_required(VERSION 3.16)
project(libsys_installer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.9.2/mingw_64")


find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui Network)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS src/headers/*.h)

set(RESOURCE 
    icons/resources.qrc
)

qt_standard_project_setup()

qt_add_executable(libsys_updater WIN32 ${SOURCES} ${HEADERS} ${RESOURCE})

target_link_libraries(libsys_updater PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
    Qt6::Network
)
