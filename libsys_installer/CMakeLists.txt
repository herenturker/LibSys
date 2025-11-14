cmake_minimum_required(VERSION 3.16)
project(libsys_installer)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui Network)

file(GLOB SOURCES src/*.cpp)
file(GLOB HEADERS src/headers/*.h)

qt_standard_project_setup()

qt_add_executable(libsys_installer ${SOURCES} ${HEADERS})

target_link_libraries(libsys_installer
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
    Qt6::Network
)
