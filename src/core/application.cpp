/** @file */
#include "core/application.hpp"

#include "core/events/event.hpp"
#include "core/map.hpp"
#include "core/window.hpp"

namespace tme {
    namespace core {

        Application::Application(const std::string& name)
            : Dispatcher(this),
              m_window(Manager<Window>::getGlobalInstance()->add(Window::create(Window::Data(this, name)))),
              m_layers(),
              m_running(true),
              m_id(uuid<Application>()) {}

        Application::~Application() {}

        void Application::update() {
            auto window = Manager<Window>::getGlobalInstance()->get(m_window);
            window->setActive();
            updateInternal();
            window->onUpdate();
        }

        void Application::onEvent(events::Event& event) {
                m_layers.onEvent(event);
                if (event.isHandled())
                    return;
                dispatchEvent<events::WindowClose>(event, &Application::handleWindowClose);
        }

        bool Application::handleWindowClose(events::WindowClose&) {
                TME_INFO("closing window");
                Manager<Window>::getGlobalInstance()->destroy(m_window);
                m_running = false;
                return true;
        }

    }
}
