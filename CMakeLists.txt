cmake_minimum_required(VERSION 3.10)

project(tme-ase-project)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CXX_EXTENSIONS OFF)

option(BUILD_DOC "Build documentation" OFF)
option(BUILD_TESTS "Build tests" OFF)

if (NOT DEFINED GCOVEXEC)
    SET(GCOVEXEC gcov)
endif()

set(BINARY tme)
set(BIN_DIR ${CMAKE_SOURCE_DIR}/bin)

add_subdirectory(src)

if(${BUILD_DOC})
    add_subdirectory(doc)
endif()

if(${BUILD_TESTS})
    add_subdirectory(test)
endif()

