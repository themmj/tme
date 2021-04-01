/** @file */
#include <iostream>
#include "core/graphics/shader.hpp"
#include "core/log.hpp"
#include "core/map.hpp"
#include "namespaces.hpp"

#include "core/events/dispatcher.hpp"
#include "core/events/window.hpp"
#include "core/events/key.hpp"
#include "core/layers/layer.hpp"
#include "core/layers/imgui.hpp"
#include "core/window.hpp"
#include "core/application.hpp"

namespace tme {
    namespace core {
        /// Example class to test event handling.
        class App : public Application {
            float m_red;
            Handle<Manager<graphics::Shader>> m_shadersHandle;
            Handle<Manager<graphics::Shader::Stage>> m_shaderStagesHandle;

            public:
            App(const std::string& name, float red) : Application(name), m_red(red) {
                if (red == 0.0f) {
                m_layers.push<layers::StyleImgui>();
                }
                m_shadersHandle = Manager<graphics::Shader>::createInstance();
                m_shaderStagesHandle = Manager<graphics::Shader::Stage>::createInstance();

                TME_INFO("creating app {}", name);

                auto vertexId = m_shaderStagesHandle->create(graphics::Shader::Stage::Type::Vertex, "/home/mmj/development/workspaces/cpp/tme/res/vertex.glsl");
                auto fragmentId = m_shaderStagesHandle->create(graphics::Shader::Stage::Type::Fragment, "/home/mmj/development/workspaces/cpp/tme/res/fragment.glsl");
                auto vertex = m_shaderStagesHandle->get(vertexId);
                auto fragment = m_shaderStagesHandle->get(fragmentId);
                auto shaderId = m_shadersHandle->create(vertex, fragment);
                auto shader  = m_shadersHandle->get(shaderId);
                shader->bind();
                shader->setUniform1i("u_Texture", 10);
                shader->setUniform1i("doesnt exist", 20);
                shader->unbind();
            }
            
            void updateInternal() override {
                glClearColor(m_red, 0.5f, 0.5f, 0.0f );
                glClear(GL_COLOR_BUFFER_BIT);
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
    tme::core::Log::init();
    tme::core::Window::init();
    TME_INFO("starting application");

    {
    auto appId = tme::core::Manager<tme::core::Application>::getGlobalInstance()->add(new tme::core::App("test", 0.0f));
    auto app = tme::core::Manager<tme::core::Application>::getGlobalInstance()->get(appId);
    auto appId1 = tme::core::Manager<tme::core::Application>::getGlobalInstance()->add(new tme::core::App("test1", 1.0f));
    auto app1 = tme::core::Manager<tme::core::Application>::getGlobalInstance()->get(appId1);

    while (app->isRunning() || app1->isRunning()) {
        if (app->isRunning())
            app->update();
        if (app1->isRunning())
            app1->update();
    }
    }
    TME_INFO("shutting down");

    tme::core::Manager<tme::core::Application>::getGlobalInstance()->clear();

    tme::core::Window::shutdown();

    return 0;
}
