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
    imgui/backends/imgui_impl_opengl3.cpp

    open-file-dialog/ImGuiFileDialog.cpp
)

target_include_directories(imgui PRIVATE
    ${INCLUDE_DIRS}
    # debian
    /usr/include/freetype2
    # apple
    /usr/local/include/freetype2
)

target_compile_options(imgui PRIVATE
    -g
    -DIMGUI_IMPL_OPENGL_LOADER_GLAD
)

