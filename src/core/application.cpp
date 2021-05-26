/** @file */
#include "core/application.hpp"

#include "core/events/event.hpp"
#include "core/events/window.hpp"
#include "core/storage.hpp"
#include "core/window.hpp"

#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"

namespace tme {
    namespace core {

        Application::Application()
            : m_id(uuid<Application>()) {}

        Application::~Application() {}

        WindowApplication::WindowApplication(const std::string& name)
            : Application(),
              Dispatcher(this),
              m_layers(),
              m_running(true) {
            m_window = Storage<Window>::global()->add(Window::create(Window::Data(this, name)))->getId();
        }

        WindowApplication::~WindowApplication() {}

        void WindowApplication::run() {
            auto window = Storage<Window>::global()->get(m_window);
            while (m_running) {
                ImGui_ImplOpenGL3_NewFrame();
                ImGui::NewFrame();
                window->update();
                render();
                if (auto imGuiDrawData = ImGui::GetDrawData(); imGuiDrawData) {
                    ImGui_ImplOpenGL3_RenderDrawData(imGuiDrawData);
                }
                ImGui::EndFrame();
                window->swapBuffer();
                window->pollEvents();
            }
        }

        void WindowApplication::onEvent(events::Event& event) {
                m_layers.onEvent(event);
                dispatchEvent<events::WindowClose>(event, &WindowApplication::handleWindowClose);
        }

        bool WindowApplication::handleWindowClose(events::WindowClose&) {
                TME_INFO("closing window");
                Storage<Window>::global()->destroy(m_window);
                m_running = false;
                return true;
        }

    }
}
