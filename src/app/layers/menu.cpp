/** @file */

#include "app/layers/menu.hpp"
#include "app/editor.hpp"

#include "glm/vec4.hpp"
#include "imgui.h"

namespace tme {
    namespace app {
        namespace layers {

            MenuBar::MenuBar(core::Identifier editorId)
                : Layer("CreationUI"),
                m_editorId(editorId) {}

            MenuBar::~MenuBar() {}

            void MenuBar::render() {
                if (ImGui::BeginMainMenuBar()) {
                    showFileOptions();
                }
            }

            void MenuBar::showFileOptions() {
                static bool openNewDialog = false;
                if (ImGui::BeginMenu("File")) {
                    if (ImGui::MenuItem("New")) {
                        openNewDialog = true;
                    }
                    ImGui::EndMenu();
                }

                if (openNewDialog) {
                    ImGui::SetNextWindowSize(ImVec2(550, 1000), ImGuiCond_FirstUseEver);
                    ImGui::Begin("New Tilemap");

                    static int32_t width;
                    static int32_t height;
                    static int32_t tileSize;

                    static float color[4];
                    color[3] = 1.0f;

                    ImGui::InputInt("Map width", &width, 1, 10);
                    ImGui::InputInt("Map height", &height, 1, 10);
                    ImGui::InputInt("Tile size", &tileSize, 1, 10);
                    ImGui::ColorPicker4("Background color", color);

                    if (ImGui::Button("Create")) {
                        auto editor = core::Storage<Editor>::global()->get(m_editorId);
                        auto tilemap = core::Storage<Tilemap>::global()->create(width, height, tileSize);
                        tilemap->setBackground(glm::vec4(color[0], color[1], color[2], color[3]));
                        editor->setTilemap(tilemap);
                        openNewDialog = false;
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Cancel")) {
                        openNewDialog = false;
                    }
                    ImGui::End();
                }
            }
                
        }
    }
}

