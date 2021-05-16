cmake_minimum_required(VERSION 3.10)

add_library(stb STATIC
    stb_image.cpp
)

target_include_directories(stb PRIVATE
    stb
)

target_compile_options(stb PRIVATE
    -g
)

