/** @file */
#include <iostream>
#include "imgui.h"
#include "namespaces.hpp"
#include "core/log.hpp"
#include "core/storage.hpp"

#include "core/events/dispatcher.hpp"
#include "core/events/window.hpp"
#include "core/events/key.hpp"
#include "core/layers/layer.hpp"
#include "core/layers/imgui.hpp"
#include "core/layers/ui.hpp"
#include "core/window.hpp"
#include "core/application.hpp"
#include "core/graphics/shader.hpp"

#include "platform/context.hpp"

namespace tme {
    namespace core {

        /// Example ui element showing the imgui demo window
        class DemoWindow : public layers::Ui {
            bool m_showWindow = true;
            public:
            DemoWindow() : layers::Ui() {}
            ~DemoWindow() {}

            void update() override {
                ImGui::ShowDemoWindow(&m_showWindow);
            }
        };

        /// Example class to test event handling.
        class App : public WindowApplication {
            float m_red;
            Handle<Storage<graphics::Shader>> m_shadersHandle;
            Handle<Storage<graphics::Shader::Stage>> m_shaderStagesHandle;

            void update() override {
                glClearColor(m_red, 0.5f, 0.5f, 0.0f );
                glClear(GL_COLOR_BUFFER_BIT);
            }

            public:
            App(const std::string& name, float red) : WindowApplication(name), m_red(red) {
                m_layers.push<DemoWindow>();
                m_layers.push<layers::Imgui>();
                m_shadersHandle = Storage<graphics::Shader>::localInstance();
                m_shaderStagesHandle = Storage<graphics::Shader::Stage>::localInstance();

                TME_INFO("creating app {}", name);

                auto vertex = m_shaderStagesHandle->create(graphics::Shader::Stage::Type::Vertex, "/home/mmj/development/workspaces/cpp/tme/res/vertex.glsl");
                auto fragment = m_shaderStagesHandle->create(graphics::Shader::Stage::Type::Fragment, "/home/mmj/development/workspaces/cpp/tme/res/fragment.glsl");
                auto shader = m_shadersHandle->create(vertex, fragment);
                shader->bind();
                shader->setUniform1i("u_Texture", 10);
                shader->setUniform1i("doesnt exist", 20);
                shader->unbind();
            }
            
            ~App() {
                m_shaderStagesHandle->clear();
                m_shadersHandle->clear();
            }
        };
    }
}

/// Program entrypoint.
int main() {
    auto contextHandle = tme::platform::Context::create();
    tme::core::Log::init();
    TME_INFO("starting application");

    auto app = tme::core::Storage<tme::core::Application>::global()->add(new tme::core::App("test", 0.0f));

    app->run();

    TME_INFO("shutting down");

    tme::core::Storage<tme::core::Application>::global()->clear();

    return 0;
}
