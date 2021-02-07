cmake_minimum_required(VERSION 3.10)

add_library(glad STATIC
    include/glad/glad.h
    include/KHR/khrplatform.h
    src/glad.c
)

target_include_directories(glad PRIVATE
    ${THIRD_PARTY_DIR}/glad/include    
)

