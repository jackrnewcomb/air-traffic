cmake_minimum_required(VERSION 3.12)
project(Core LANGUAGES CXX)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)


# Collect source and header files
file(GLOB_RECURSE SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
)

# Create a library from these files
add_library(core ${SOURCES})
include_directories(${PROJECT_SOURCE_DIR}/../3rd/SFML/include)
link_directories(${PROJECT_SOURCE_DIR}/../3rd/SFML/lib)
target_link_libraries(core PUBLIC sfml-graphics)

target_include_directories(core PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# Path to SFML DLLs in the build tree
set(SFML_DLL_DIR "${CMAKE_BINARY_DIR}/3rd/SFML/lib/Debug")

# Copy DLLs to the output folder after build
add_custom_command(TARGET core POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${SFML_DLL_DIR}"
    $<TARGET_FILE_DIR:core>
)
