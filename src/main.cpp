/** @file */
#include <iostream>
#include "core/log.hpp"
#include "namespaces.hpp"

#include "glad/glad.h"
#include "core/events/dispatcher.hpp"
#include "core/events/window.hpp"
#include "core/events/key.hpp"
#include "core/layers/layer.hpp"
#include "core/layers/imgui.hpp"
#include "core/window.hpp"

namespace tme {
    namespace core {
        /// Example class to test event handling.
        class App : public events::Dispatcher<App> {
            bool m_running;

            float red = 0.0f, green = 0.0f, blue = 0.0f;

            layers::Stack m_layers;

            bool handleWindowClose(events::WindowClose&) {
                m_running = false;
                return true;
            }

            bool handleKeyPress(events::KeyPress& event) {
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
                return false;
            }

            public:
            App() : Dispatcher(this), m_running(true) {}

            /// event callback function
            void onEvent(events::Event& event) override {
                m_layers.onEvent(event);
                if (event.getType() != events::Type::WindowUpdate && event.getType() != events::Type::MouseMove)
                    TME_INFO("received event {}", event);
                if (event.isHandled())
                    return;
                dispatchEvent<events::WindowClose>(event, &App::handleWindowClose);
                dispatchEvent<events::KeyPress>(event, &App::handleKeyPress);
            }

            /// run application
            void run() {
                auto window = Window::create({"This is a test", 640, 300, this, true});
                m_layers.push<layers::DemoImgui>();
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
