cmake_minimum_required(VERSION 3.27)
project(untitled C)

set(CMAKE_C_STANDARD 11)

find_package(SDL2)
find_package(SDL2_image)

add_executable(untitled main.c)
add_library(cowmark cowmark.c
        cowmark.c)

target_link_libraries(untitled SDL2::SDL2)
target_link_libraries(untitled SDL2_image::SDL2_image)
target_link_libraries(untitled cowmark)

file(COPY assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})