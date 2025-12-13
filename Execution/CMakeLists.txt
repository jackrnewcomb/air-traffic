cmake_minimum_required(VERSION 3.12)
project(Execution LANGUAGES CXX)

# Collect main source file
file(GLOB_RECURSE SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
)

# Create executable
add_executable(execute ${SOURCES})

# Link to your libraries
target_link_libraries(execute PRIVATE core aircraft)
