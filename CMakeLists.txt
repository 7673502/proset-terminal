cmake_minimum_required(VERSION 3.31.5)

project(proset VERSION 1.1
               DESCRIPTION "ProSet in C"
               LANGUAGES C)

# show all warnings
add_compile_options(-Wall -Wextra -Wpedantic)

add_executable(${PROJECT_NAME} src/main.c)

set_target_properties(proset PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
