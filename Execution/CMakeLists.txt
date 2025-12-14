cmake_minimum_required(VERSION 3.12)
project(Execution LANGUAGES CXX)

# Collect main source file
file(GLOB_RECURSE SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
)

# Create executable
add_executable(execute ${SOURCES})

# Link to your libraries
target_link_libraries(execute PRIVATE core aircraft atc)
add_dependencies(execute core aircraft atc)

set(SFML_DLL_DIR "${CMAKE_BINARY_DIR}/3rd/SFML/lib/Debug")

add_custom_command(TARGET execute POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${SFML_DLL_DIR}"
    $<TARGET_FILE_DIR:execute>
)

file(GLOB PLUGIN_DLLS "${CMAKE_BINARY_DIR}/Plugins/*/Debug/*.dll")

add_custom_command(TARGET execute POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${PLUGIN_DLLS}
    $<TARGET_FILE_DIR:execute>
)

set(CORE_DLL_DIR "${CMAKE_BINARY_DIR}/Core/Debug")

add_custom_command(TARGET execute POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CORE_DLL_DIR}"
    $<TARGET_FILE_DIR:execute>
)
