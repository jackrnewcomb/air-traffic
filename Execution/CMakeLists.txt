cmake_minimum_required(VERSION 3.12)
project(Execution LANGUAGES CXX)

# Collect main source file
file(GLOB_RECURSE SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
)

# Create executable
add_executable(execute ${SOURCES})

# Link to your libraries
target_link_libraries(execute PRIVATE core aircraft atc navigation)

file(GLOB PLUGIN_DLLS CONFIGURE_DEPENDS
    "${CMAKE_BINARY_DIR}/Plugins/*/Debug/*.dll"
)

foreach(dll ${PLUGIN_DLLS})
    add_custom_command(TARGET execute POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${dll}"
        $<TARGET_FILE_DIR:execute>
    )
endforeach()


add_custom_command(TARGET execute POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_BINARY_DIR}/3rd/SFML/lib/Debug"
        $<TARGET_FILE_DIR:execute>
)