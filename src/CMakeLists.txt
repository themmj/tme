set (SRC_FILES
    main.cpp
    core/log.cpp
    core/window.cpp
    core/layers/layer.cpp
    platform/glfw.cpp
    core/application.cpp
    core/layers/imgui.cpp
    core/graphics/common.cpp
    core/graphics/shader.cpp
    core/graphics/vertex.cpp
    core/graphics/index.cpp
    core/graphics/buffer.cpp
    core/graphics/texture.cpp
    core/graphics/batch.cpp
)

set (APP_SRC_FILES
    app/graphics/tile.cpp
    app/graphics/color.cpp
    app/graphics/texture.cpp
    app/layers/background.cpp
    app/layers/menu.cpp
    app/layers/map.cpp
    app/layers/editing.cpp
    app/layers/ui.cpp
    app/camera.cpp
    app/tilemap.cpp
    app/editor.cpp
)

# potentially inject platform specific implementations
if(APPLE)
    message(STATUS "using platform implementation for apple")
    set(SRC_FILES
        ${SRC_FILES}
        platform/glfw.cpp
    )
elseif(UNIX)
    message(STATUS "using platform implementation for linux")
    set(SRC_FILES
        ${SRC_FILES}
        platform/glfw.cpp
    )
else()
    message(FATAL_ERROR "unsupported platform")
endif()

add_executable(${BINARY} ${SRC_FILES} ${APP_SRC_FILES})

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
    WORKING_DIRECTORY ${BIN_DIR}
)

add_dependencies(run ${BINARY})

