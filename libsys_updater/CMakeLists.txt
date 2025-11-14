cmake_minimum_required(VERSION 3.16)
project(libsys_updater)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 COMPONENTS REQUIRED Core Widgets Gui Network)

file(GLOB SOURCES src/*.cpp)
file(GLOB HEADERS src/*.h)

qt_standard_project_setup()

qt_add_executable(libsys_updater ${SOURCES} ${HEADERS})

target_link_libraries(libsys_updater Qt6::Core Qt6::Widgets Qt6::Gui Qt6::Network)
