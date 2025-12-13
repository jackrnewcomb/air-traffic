# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)


# Project name and version
project(air-traffic VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(Execution)
add_subdirectory(Core)
add_subdirectory(Plugins)
add_subdirectory(3rd)