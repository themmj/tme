#### Gtest
add_extern_directory(googletest)

mark_as_advanced(
    BUILD_GMOCK BUILD_GTEST BUILD_SHARED_LIBS
    gmock_build_tests gtest_build_samples gtest_build_tests
    gtest_disable_pthreads gtest_force_shared_crt gtest_hide_internal_symbols
)

set_target_properties(gtest PROPERTIES FOLDER extern)
set_target_properties(gtest_main PROPERTIES FOLDER extern)
set_target_properties(gmock PROPERTIES FOLDER extern)
set_target_properties(gmock_main PROPERTIES FOLDER extern)

#### TestBuild
add_executable(${BINARY}-test main_test.cpp)

target_include_directories(${BINARY}-test PUBLIC
    ${INCLUDE_DIRS}
)
target_link_libraries(${BINARY}-test
    gtest
    gmock
    gtest_main
    ${BINARY}-lib
)

set_target_properties(
   ${BINARY}-test PROPERTIES

   RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}
)

target_compile_options(
    ${BINARY}-test PUBLIC
    ${TEST_COMPILE_OPTIONS}
)

target_link_options(
    ${BINARY}-test PUBLIC
    ${TEST_LINK_OPTIONS}
)

add_custom_target(test
    COMMAND ${BIN_DIR}/${BINARY}-test
)

add_custom_target(clean-gcno
    COMMAND find . -name "\"*.gcno\"" -type f -delete
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_target(clean-gcda
    COMMAND find . -name "\"*.gcda\"" -type f -delete
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_target(coverage
    COMMAND mkdir -p ./test/coverage
    COMMAND gcovr -r . -f src/ -e "\'(.+\/)?(main|.+_test)\.cpp\'" --branches --html --html-details -o ${CMAKE_SOURCE_DIR}/test/coverage/coverage.html --gcov-executable ${GCOVEXEC}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
add_dependencies(coverage test)

