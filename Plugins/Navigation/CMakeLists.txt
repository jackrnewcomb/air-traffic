cmake_minimum_required(VERSION 3.12)
project(Aircraft LANGUAGES CXX)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)


# Collect source and header files
file(GLOB_RECURSE SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
)

# Create a library from these files
add_library(navigation ${SOURCES})
target_link_libraries(navigation PRIVATE core)

target_include_directories(navigation PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
