/** @file */
#include "core/layers/imgui.hpp"
#include "core/events/key.hpp"
#include "core/events/window.hpp"
#include "core/log.hpp"
#include "core/events/event.hpp"
#include "core/events/mouse.hpp"

#include "glad/glad.h"
#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"

namespace tme {
    namespace core {
        namespace layers {
            
            void Imgui::onEvent(events::Event& event) {
                dispatchEvent<events::KeyPress>(event, &Imgui::handleKeyPress);
                dispatchEvent<events::KeyRelease>(event, &Imgui::handleKeyRelease);
                dispatchEvent<events::KeyChar>(event, &Imgui::handleKeyChar);
                dispatchEvent<events::MouseKeyPress>(event, &Imgui::handleMouseKeyPress);
                dispatchEvent<events::MouseKeyRelease>(event, &Imgui::handleMouseKeyRelease);
                dispatchEvent<events::MouseMove>(event, &Imgui::handleMouseMove);
                dispatchEvent<events::MouseScroll>(event, &Imgui::handleMouseScroll);
                dispatchEvent<events::WindowUpdate>(event, &Imgui::handleWindowUpdate);
                dispatchEvent<events::WindowResize>(event, &Imgui::handleWindowResize);
            }

            // the following callbacks are excluded from coverage tests as then cannot be tested with
            // automated unit tests because then i.e. depend on user input etc.
            // rest assured they are tested thoroughly in a manual fashion
            // GCOVR_EXCL_START
            bool Imgui::handleKeyPress(events::KeyPress& event) {
                ImGuiIO& io = ImGui::GetIO();
                io.KeysDown[event.getKey().getKeyCode()] = true;
                return io.WantCaptureKeyboard;
            }
            bool Imgui::handleKeyRelease(events::KeyRelease& event) {
                ImGuiIO& io = ImGui::GetIO();
                io.KeysDown[event.getKey().getKeyCode()] = false;

                io.KeyCtrl = event.getKey().hasModControl();
                io.KeyShift = event.getKey().hasModShift();
                io.KeyAlt = event.getKey().hasModAlt();
                io.KeySuper = event.getKey().hasModSuper();

                return io.WantCaptureKeyboard;
            }
            bool Imgui::handleKeyChar(events::KeyChar& event) {
                ImGuiIO& io = ImGui::GetIO();
                auto keyCode = event.getKey().getKeyCode();
                if (keyCode > 0 && keyCode < 0x10000) {
                    io.AddInputCharacter(static_cast<unsigned int>(keyCode));
                }
                return io.WantCaptureKeyboard;
            }
            bool Imgui::handleMouseKeyPress(events::MouseKeyPress& event) {
                ImGuiIO& io = ImGui::GetIO();
                io.MouseDown[event.getKey().getKeyCode()] = true;
                return io.WantCaptureMouse;
            }
            bool Imgui::handleMouseKeyRelease(events::MouseKeyRelease& event) {
                ImGuiIO& io = ImGui::GetIO();
                io.MouseDown[event.getKey().getKeyCode()] = false;
                return io.WantCaptureMouse;
            }
            bool Imgui::handleMouseMove(events::MouseMove& event) {
                ImGuiIO& io = ImGui::GetIO();
                io.MousePos = ImVec2(static_cast<float>(event.getXPos()), static_cast<float>(event.getYPos()));
                return io.WantCaptureMouse;
            }
            bool Imgui::handleMouseScroll(events::MouseScroll& event) {
                ImGuiIO& io = ImGui::GetIO();
                io.MouseWheel = static_cast<float>(event.getYOffset());
                io.MouseWheelH = static_cast<float>(event.getXOffset());
                return io.WantCaptureMouse;
            }
            // GCOVR_EXCL_STOP
            
            bool Imgui::handleWindowUpdate(events::WindowUpdate& event) {
                ImGuiIO& io = ImGui::GetIO();
                io.DeltaTime = static_cast<float>(event.getDeltaTime());
                
                update();

                return false;
            }

            // same reasoning as above
            // GCOVR_EXCL_START
            bool Imgui::handleWindowResize(events::WindowResize& event) {
                ImGuiIO& io = ImGui::GetIO();
                io.DisplaySize = ImVec2(static_cast<float>(event.getWidth()), static_cast<float>(event.getHeight()));
                io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
                glViewport(0, 0, (int) event.getWidth(), (int) event.getHeight());
                return false;
            }


            void DemoImgui::update() {
                static bool show = true;
                ImGui::ShowDemoWindow(&show);
            }
            // GCOVR_EXCL_STOP

        }
    }
}
