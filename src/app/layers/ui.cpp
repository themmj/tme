/** @file */

#include "app/layers/ui.hpp"
#include <sstream>
#include <stdexcept>
#include "app/graphics/tile.hpp"
#include "core/exceptions/input.hpp"
#include "core/graphics/shader.hpp"
#include "core/graphics/texture.hpp"
#include "core/storage.hpp"

#include "glm/vec4.hpp"
#include "imgui.h"
#include "ImGuiFileDialog.h"

namespace tme {
    namespace app {
        namespace layers {

            EditingUI::EditingUI(core::Handle<Tilemap> tilemap)
                : Layer("EditingUI"),
                m_tilemap(tilemap),
                m_errorOccurred(false),
                m_error("No error", "Will be overwritten in actual error cases") {
                m_colorTileFactory = core::Handle<graphics::ColorTileFactory>(new graphics::ColorTileFactory());
                m_textureTileFactory = core::Handle<graphics::TextureTileFactory>(new graphics::TextureTileFactory(0));
                m_tilemap->getCursor()->tileFactory = m_colorTileFactory;
            }

            EditingUI::~EditingUI() {}

            void EditingUI::render() {
                ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);
                ImGui::Begin("TME Editor");

                showLayerSelection();
                ImGui::Separator();
                showTileSelection();

                ImGui::End();

                showErrors();
            }

            void EditingUI::showLayerSelection() {
                ImGui::Text("Layer:");
                ImGui::Indent();
                if (ImGui::Button("+")) {
                    m_tilemap->addLayer();
                }
                if (m_tilemap->getLayerCount() > 1) {
                    ImGui::SameLine();
                    if (ImGui::Button("-")) {
                        m_tilemap->removeLayer();
                    }
                }
                static size_t selected = 0;
                for (size_t i = 0; i < m_tilemap->getLayerCount(); ++i) {
                    size_t layerIndex = m_tilemap->getLayerCount() - 1 - i;
                    std::ostringstream ss;
                    ss << "Layer " << layerIndex;
                    if (ImGui::Selectable(ss.str().c_str(), selected == layerIndex)) {
                        selected = layerIndex;
                    }
                }
                m_tilemap->getCursor()->layer = selected;
            }

            void EditingUI::showTileSelection() {
                ImGui::Unindent();
                ImGui::Text("Tile:");
                ImGui::Indent();
                static int selected = 0;
                ImGui::RadioButton("Colored Tile", &selected, 0);
                ImGui::SameLine();
                ImGui::RadioButton("Textured Tile", &selected, 1);
                if (selected == 0)
                    showColorTileSelection();
                else if (selected == 1)
                    showTextureTileSelection();
            }

            void EditingUI::showColorTileSelection() {
                m_tilemap->getCursor()->tileFactory = m_colorTileFactory;
                
                showShaderSelection(m_colorTileFactory);

                static float color[4] = {0.0f, 0.0f, 0.0f, 1.0f};
                ImGui::ColorPicker4("Color", color);
                m_colorTileFactory->setColor(glm::vec4(color[0], color[1], color[2], color[3]));
            }

            void EditingUI::showTextureTileSelection() {
                m_tilemap->getCursor()->tileFactory = m_textureTileFactory;

                showShaderSelection(m_textureTileFactory);
                showTextureSelection();

                // early exit when no texture is attached to the project
                if (!core::Storage<core::graphics::Texture>::global()->has(m_textureTileFactory->getTexture())) {
                    return;
                }

                static double frameTime = 0.0;
                ImGui::InputDouble("Frame time in seconds", &frameTime, 0.1, 1.0);
                static ImVec2 imageSize = ImVec2(64.0, 64.0);
                static ImVec4 bg = ImVec4(0, 0, 0, 0);
                static glm::vec4 textureCoordinates;
                auto activeTexture = core::Storage<core::graphics::Texture>::global()->get(m_textureTileFactory->getTexture());
                double  widthStep = static_cast<double>(m_tilemap->getTileSize()) / static_cast<double>(activeTexture->getWidth());
                double  heightStep = static_cast<double>(m_tilemap->getTileSize()) / static_cast<double>(activeTexture->getHeight());
                int i = 0;
                static int selected = -1;
                for (double y = 1.0 - heightStep; y >= 0.0; y -= heightStep) {
                    for (double x = 0.0; x <= 1.0 - widthStep; x += widthStep) {
                        glm::vec4 texCoords = glm::vec4(x, y, x + widthStep, y + heightStep);
                        ImGui::PushID(i);
                        ImVec2 topLeft = ImVec2(texCoords.x, texCoords.w);
                        ImVec2 bottomRight = ImVec2(texCoords.z, texCoords.y);
                        ImVec4 tint = (i == selected) ? ImVec4(1.0f, 1.0f, 1.0f, 0.8f) : ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
                        if (ImGui::ImageButton((void*)(intptr_t)activeTexture->getId(), imageSize, topLeft, bottomRight, -1, bg, tint)) {
                            textureCoordinates = texCoords;
                            selected = i;
                        }
                        ImGui::PopID();
                        ++i;
                        ImGui::SameLine();
                    }
                    ImGui::NewLine();
                }
                if (ImGui::Button("Add frame")) {
                    m_textureTileFactory->addFrame({frameTime, textureCoordinates});
                }
                i = 0;
                for (const auto frame : m_textureTileFactory->getFrames()) {
                    ImGui::PushID(i);
                    ImVec2 topLeft = ImVec2(frame.texPos.x, frame.texPos.w);
                    ImVec2 bottomRight = ImVec2(frame.texPos.z, frame.texPos.y);
                    if (ImGui::Button("X")) {
                        m_textureTileFactory->removeFrame(i);
                    }
                    ImGui::SameLine();
                    ImGui::Image((void*)(intptr_t)activeTexture->getId(), imageSize, topLeft, bottomRight);
                    ImGui::SameLine();
                    ImGui::Text("Duration: %f", frame.time);
                    ImGui::NewLine();
                    ImGui::PopID();
                    ++i;
                }
            }

            void EditingUI::showShaderSelection(core::Handle<graphics::TileFactory> factory) {
                auto globalShaderHandle = core::Storage<core::graphics::Shader>::global();
                // does not check because default shader should be provided
                auto selectedShader = globalShaderHandle->get(factory->getShader());
                if (ImGui::BeginCombo("Shader", selectedShader->getName().c_str())) {
                    auto selectableLoop = [=](IdentifierVector shaderIds) {
                        for (auto shaderId : shaderIds) {
                            bool selected = selectedShader->getId() == shaderId;
                            auto shader = globalShaderHandle->get(shaderId);
                            if (ImGui::Selectable(shader->getName().c_str(), selected)) {
                                factory->setShader(shaderId);
                            }
                            if (selected) {
                                ImGui::SetItemDefaultFocus();
                            }
                        }
                    };
                    selectableLoop(m_tilemap->getShaderIds());
                    selectableLoop(Defaults::instance()->shaders.getVector());
                    ImGui::EndCombo();
                }
            }

            void EditingUI::showTextureSelection() {
                auto globalTextureHandle = core::Storage<core::graphics::Texture>::global();
                // if the factory has no valid texture set, there are no textures to choose from
                if (globalTextureHandle->has(m_textureTileFactory->getTexture())) {
                    auto selectedTexture = globalTextureHandle->get(m_textureTileFactory->getTexture());
                    if (ImGui::BeginCombo("Texture", selectedTexture->getFilePath().c_str())) {
                        auto selectableLoop = [=](IdentifierVector textureIds) {
                            for (auto textureId : textureIds) {
                                bool selected = selectedTexture->getId() == textureId;
                                auto texture = globalTextureHandle->get(textureId);
                                if (ImGui::Selectable(texture->getFilePath().c_str(), selected)) {
                                    m_textureTileFactory->setTexture(textureId);
                                }
                                if (selected) {
                                    ImGui::SetItemDefaultFocus();
                                }
                            }
                        };
                        selectableLoop(m_tilemap->getTextureIds());
                        selectableLoop(Defaults::instance()->textures.getVector());
                        ImGui::EndCombo();
                    }
                    ImGui::SameLine();
                }
                if (ImGui::Button("Add texture")) {
                    ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose image file", ".png,.bmp,.gif", ".");
                }
                if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
                    if (ImGuiFileDialog::Instance()->IsOk()) {
                        std::string filePath = ImGuiFileDialog::Instance()->GetFilePathName();;
                        try {
                            auto texture = core::Storage<core::graphics::Texture>::global()->create(filePath);
                            m_tilemap->addTexture(texture->getId());
                            m_textureTileFactory->setTexture(texture->getId());
                        } catch(const core::exceptions::InvalidInput& e) {
                            m_errorOccurred = true;
                            m_error = e;
                        } CATCH_ALL
                    }
                    ImGuiFileDialog::Instance()->Close();
                }
            }

            void EditingUI::showErrors() {
                if (m_errorOccurred) {
                    ImGui::Begin(m_error.type());
                    ImGui::TextUnformatted(m_error.what());
                    if (ImGui::Button("Ok")) {
                        m_errorOccurred = false;
                    }
                    ImGui::End();
                }
            }

        }
    }
}

