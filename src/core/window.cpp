/** @file */
#include "core/window.hpp"
#include "core/key.hpp"
#include "imgui.h"
#include <sstream>

namespace tme {
    namespace core {

        Window::Window(const Data& data) : m_id(uuid<Window>()), m_data(data) { 
            m_data.m_this = this;

            // ImGui init
            m_imGuiContext = ImGui::CreateContext();
            ImGui::StyleColorsDark();

            ImGuiIO& io = ImGui::GetIO();
            io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
            io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

            io.KeyMap[ImGuiKey_Tab] = TME_KEY_TAB;
            io.KeyMap[ImGuiKey_LeftArrow] = TME_KEY_LEFT;
            io.KeyMap[ImGuiKey_RightArrow] = TME_KEY_RIGHT;
            io.KeyMap[ImGuiKey_UpArrow] = TME_KEY_UP;
            io.KeyMap[ImGuiKey_DownArrow] = TME_KEY_DOWN;
            io.KeyMap[ImGuiKey_PageUp] = TME_KEY_PAGE_UP;
            io.KeyMap[ImGuiKey_PageDown] = TME_KEY_PAGE_DOWN;
            io.KeyMap[ImGuiKey_Home] = TME_KEY_HOME;
            io.KeyMap[ImGuiKey_End] = TME_KEY_END;
            io.KeyMap[ImGuiKey_Insert] = TME_KEY_INSERT;
            io.KeyMap[ImGuiKey_Delete] = TME_KEY_DELETE;
            io.KeyMap[ImGuiKey_Backspace] = TME_KEY_BACKSPACE;
            io.KeyMap[ImGuiKey_Space] = TME_KEY_SPACE;
            io.KeyMap[ImGuiKey_Enter] = TME_KEY_ENTER;
            io.KeyMap[ImGuiKey_Escape] = TME_KEY_ESCAPE;
            io.KeyMap[ImGuiKey_A] = TME_KEY_A;
            io.KeyMap[ImGuiKey_C] = TME_KEY_C;
            io.KeyMap[ImGuiKey_V] = TME_KEY_V;
            io.KeyMap[ImGuiKey_X] = TME_KEY_X;
            io.KeyMap[ImGuiKey_Y] = TME_KEY_Y;
            io.KeyMap[ImGuiKey_Z] = TME_KEY_Z;
        }

        Window::~Window() {
            if (m_imGuiContext) {
                ImGui::DestroyContext(m_imGuiContext);
            }
        }

        void Window::setActive() {
            TME_INFO("{} is now active with context {}", *this, (void*)m_imGuiContext);
            setActiveInternal();
            ImGui::SetCurrentContext(m_imGuiContext);
        }

        std::string Window::toString() const {
            std::stringstream ss;
            ss << "Window: " << getId() << '(' << getTitle() << ',' << getWidth() << ',' << getHeight() << ',' << isVSync() << ')';
            return ss.str();
        }

        void Window::setTitle(const std::string& title) {
            setTitleInternal(title);
            m_data.title = title;
        }

        void Window::setVSync(bool enable) {
            setVSyncInternal(enable);
            m_data.vSyncEnabled = enable;
        }

    }
}
