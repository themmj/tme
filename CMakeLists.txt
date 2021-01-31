cmake_minimum_required(VERSION 3.10)

project(tme-ase-project)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_DOC "Build documentation" OFF)
option(BUILD_TESTS "Build tests" OFF)
option(DEBUG_BUILD "Build in debug mode" OFF)

# general settings
set(BINARY tme)
set(BIN_DIR ${CMAKE_SOURCE_DIR}/bin)
set(THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/third-party)

# gcov binary
if (NOT DEFINED GCOVEXEC)
    SET(GCOVEXEC gcov)
endif()

# helper to add third party cmake file
macro(add_extern_directory)
    add_subdirectory(${THIRD_PARTY_DIR}/${ARGV0} third-party/${ARGV0})
endmacro()

# spdlog
add_extern_directory(spdlog)

# opengl
set(OpenGL_GL_PREFERENCE GLVND)
find_package( OpenGL REQUIRED )

# glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_extern_directory(glfw)

# include dirs for bin and test
set(INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/src
    ${THIRD_PARTY_DIR}/spdlog/include
    ${THIRD_PARTY_DIR}/googletest/googletest/include
    ${THIRD_PARTY_DIR}/googletest/googlemock/include
    ${OPENGL_INCLUDE_DIRS}
)

# libraries
set(LIBRARIES
    spdlog
    glfw
    OpenGL::GL
)

# options for bin
if(${DEBUG_BUILD})
    set(BIN_COMPILE_OPTIONS
        -DTME_DEBUG
        -O0
        -g
    )
else()
    set(BIN_COMPILE_OPTIONS
        -OFast
    )
endif()
set(BIN_COMPILE_OPTIONS
    ${BIN_COMPILE_OPTIONS}
    -std=c++17
    -Wall
    -Werror
    -Wextra
    -Wpedantic
    -Wconversion
    -Wcast-align
    -Wunused
    -Wshadow
)

# options for test
set(TEST_COMPILE_OPTIONS
    ${BIN_COMPILE_OPTIONS}
    -fprofile-arcs
    -ftest-coverage
)
set(TEST_LINK_OPTIONS
    --coverage
)

add_subdirectory(src)

if(${BUILD_DOC})
    add_subdirectory(doc)
endif()

if(${BUILD_TESTS})
    add_subdirectory(test)
endif()

