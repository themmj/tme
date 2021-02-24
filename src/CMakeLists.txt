set (SRC_FILES
    main.cpp
    core/log.cpp
    core/window.cpp
    platform/glfw.cpp
    ../third-party/imgui/imgui/backends/imgui_impl_opengl3.cpp
    core/layers/imgui.cpp
)

add_executable(${BINARY} ${SRC_FILES})

target_include_directories(${BINARY} PUBLIC
    ${INCLUDE_DIRS}
)

target_link_libraries(${BINARY}
    ${LIBRARIES}
)

target_compile_options(${BINARY} PUBLIC
    ${BIN_COMPILE_OPTIONS}
)

set_target_properties(
    ${BINARY} PROPERTIES

    # set output to bin dir
    RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}
)

add_library(${BINARY}-lib STATIC EXCLUDE_FROM_ALL ${SRC_FILES})

target_include_directories(${BINARY}-lib PUBLIC
    ${INCLUDE_DIRS}
)

target_link_libraries(${BINARY}-lib
    ${LIBRARIES}
)

target_compile_options(${BINARY}-lib PUBLIC
    ${TEST_COMPILE_OPTIONS}
)

target_link_options(
    ${BINARY}-lib PUBLIC
    ${TEST_LINK_OPTIONS}
)

add_custom_target(run
    COMMAND ${BIN_DIR}/${BINARY}
)

add_dependencies(run ${BINARY})

