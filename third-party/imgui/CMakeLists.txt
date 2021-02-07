cmake_minimum_required(VERSION 3.10)

add_library(imgui STATIC
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui.cpp
    imgui/imgui_widgets.cpp
    imgui/misc/cpp/imgui_stdlib.cpp
    imgui/misc/freetype/imgui_freetype.cpp
    imgui/misc/fonts/binary_to_compressed_c.cpp
)

target_include_directories(imgui PRIVATE
    ${THIRD_PARTY_DIR}/imgui/imgui
    /usr/include/freetype2
)

