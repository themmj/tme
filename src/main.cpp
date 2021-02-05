/** @file */
#include <iostream>
#include "namespaces.hpp"
#include "core/log.hpp"

#include "core/events/dispatcher.hpp"
#include "platform/glfw.hpp"
#include "core/events/window.hpp"
#include "core/events/key.hpp"

namespace tme {
    namespace core {
        /// Example class to test event handling.
        class App : public events::Dispatcher<App> {
            bool m_running;

            float red, green, blue;

            bool handleWindowClose(events::WindowClose&) {
                m_running = false;
                return true;
            }

            bool handleKeyPress(events::KeyPressed& event) {
                switch(event.getKey().getKeyCode()) {
                    case TME_KEY_R:
                        red += 0.1f;
                        if (red > 1.0f)
                            red = 0.0f;
                        break;
                    case TME_KEY_G:
                        green += 0.1f;
                        if (green > 1.0f)
                            green = 0.0f;
                        break;
                    case TME_KEY_B:
                        blue += 0.1f;
                        if (blue > 1.0f)
                            blue = 0.0f;
                        break;
                }
                return true;
            }

            public:
            App() : Dispatcher(this), m_running(true) {}

            /// event callback function
            void onEvent(events::Event& event) override {
                TME_INFO("received event {}", event);
                dispatchEvent<events::WindowClose>(event, &App::handleWindowClose);
                dispatchEvent<events::KeyPressed>(event, &App::handleKeyPress);
            }

            /// run application
            void run() {
                auto window = Window::create({"This is a test", 640, 300, this});
                while (m_running) {
                    glClearColor( red, green, blue, 0.0f );
                    glClear(GL_COLOR_BUFFER_BIT);
                    window->onUpdate();
                }
                delete window;
            }
        };
    }
}

/// Program entrypoint.
int main() {
    tme::core::Log::init();
    TME_INFO("starting application");

    tme::core::App app;
    app.run();

    TME_INFO("shutting down");

    return 0;
}
