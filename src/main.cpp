/** @file */
#include <iostream>
#include "core/exceptions/validation.hpp"
#include "core/graphics/texture.hpp"
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
#include "core/graphics/vertex.hpp"
#include "core/graphics/index.hpp"
#include "core/graphics/buffer.hpp"
#include "core/exceptions/input.hpp"

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
            float m_red, m_redIncr;
            Handle<Storage<graphics::Shader>> m_shadersHandle;
            Handle<Storage<graphics::Shader::Stage>> m_shaderStagesHandle;
            Handle<Storage<graphics::VertexLayout>> m_vlHandle;
            Handle<Storage<graphics::VertexBuffer>> m_vbHandle;
            Handle<Storage<graphics::VertexArray>> m_vaHandle;
            Handle<Storage<graphics::IndexBuffer>> m_ibHandle;
            Handle<Storage<graphics::Texture>> m_texHandle;

            Identifier m_va;
            Identifier m_shader;
            Identifier m_ib;
            Identifier m_tva;
            Identifier m_tshader;
            Identifier m_texture;

            void update() override {
                glClearColor(0.2f, 0.5f, 0.2f, 1.0f );
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                auto va = m_vaHandle->get(m_va);
                auto ib = m_ibHandle->get(m_ib);
                auto shader = m_shadersHandle->get(m_shader);

                shader->bind();
                shader->setUniform4f("u_color", m_red, 0.0f, 0.0f, 0.5f);
                va->bind();
                ib->bind();

                glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(ib->getPrimitiveCount()), GL_UNSIGNED_INT, nullptr);

                auto tva = m_vaHandle->get(m_tva);
                auto tshader = m_shadersHandle->get(m_tshader);
                auto texture = m_texHandle->get(m_texture);

                texture->setActive(0);
                texture->bind();
                tshader->bind();
                tshader->setUniform1i("u_texture", 0);
                tva->bind();
                ib->bind();

                glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(ib->getPrimitiveCount()), GL_UNSIGNED_INT, nullptr);

                m_red += m_redIncr;
                if (m_red < 0.0f || m_red > 1.0f) {
                    m_redIncr = -m_redIncr;
                }
            }

            struct Pos {
                float x, y;
            };

            struct TexturedPos {
                Pos pos, texPos;
            };

            struct Index {
                unsigned int first, second, third;
            };

            public:
            App(const std::string& name, float red) : WindowApplication(name), m_red(red), m_redIncr(0.005f) {
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                m_layers.push<DemoWindow>();
                m_layers.push<layers::Imgui>();
                m_shadersHandle = Storage<graphics::Shader>::localInstance();
                m_shaderStagesHandle = Storage<graphics::Shader::Stage>::localInstance();
                m_vlHandle = Storage<graphics::VertexLayout>::localInstance();
                m_vbHandle = Storage<graphics::VertexBuffer>::localInstance();
                m_vaHandle = Storage<graphics::VertexArray>::localInstance();
                m_ibHandle = Storage<graphics::IndexBuffer>::localInstance();
                m_texHandle = Storage<graphics::Texture>::localInstance();

                TME_INFO("creating app {}", name);
                auto layout = m_vlHandle->create();
                layout->push<float>(2);

                auto vb = m_vbHandle->create(sizeof(Pos), 3);
                Pos pos[3];
                pos[0] = {-0.5f, -0.5f};
                pos[1] = { 0.5f, -0.5f};
                pos[2] = {-0.5f,  0.5f};
                auto res = vb->add(3, &pos[0]);
                if (res.offset == graphics::INVALID_OFFSET) {
                    TME_WARN("could not add verticies to buffer");
                }

                auto va = m_vaHandle->create(vb, layout);

                auto ib = m_ibHandle->create(3, sizeof(Index), 1);
                Index index{0, 1, 2};
                res = ib->add(1, &index);
                if (res.offset == graphics::INVALID_OFFSET) {
                    TME_WARN("could not add index to buffer");
                }

                auto tlayout = m_vlHandle->create();
                tlayout->push<float>(2);
                tlayout->push<float>(2);

                auto tvb = m_vbHandle->create(sizeof(TexturedPos), 3);
                TexturedPos tpos[3];
                tpos[0] = {{-0.5f, 0.5f},{ 0.0f, 1.0f}};
                tpos[1] = {{ 0.5f, 0.5f},{ 1.0f, 1.0f}};
                tpos[2] = {{ 0.5f,-0.5f},{ 1.0f, 0.0f}};
                res = tvb->add(3, &tpos[0]);
                if (res.offset == graphics::INVALID_OFFSET) {
                    TME_WARN("could not add verticies to buffer");
                }

                auto tva = m_vaHandle->create(tvb, tlayout);

                try {
                    auto vertex = m_shaderStagesHandle->create(graphics::Shader::Stage::Type::Vertex, "default", "/home/mmj/development/workspaces/cpp/tme/res/vertex.glsl");
                    auto fragment = m_shaderStagesHandle->create(graphics::Shader::Stage::Type::Fragment, "default", "/home/mmj/development/workspaces/cpp/tme/res/fragment.glsl");
                    auto shader = m_shadersHandle->create(vertex, fragment);
                    auto tvertex = m_shaderStagesHandle->create(graphics::Shader::Stage::Type::Vertex, "textured", "/home/mmj/development/workspaces/cpp/tme/res/vertex-texture.glsl");
                    auto tfragment = m_shaderStagesHandle->create(graphics::Shader::Stage::Type::Fragment, "textured", "/home/mmj/development/workspaces/cpp/tme/res/fragment-texture.glsl");
                    auto tshader = m_shadersHandle->create(tvertex, tfragment);
                    auto texture = m_texHandle->create("/home/mmj/development/workspaces/cpp/tme/res/gh-skin.png");
                    m_shader = shader->getId();
                    m_tshader = tshader->getId();
                    m_texture = texture->getId();

                } catch(const exceptions::InvalidInput& e) {
                    TME_ASSERT(false, "{}: {}", e.type(), e.what());
                } catch(const exceptions::SyntaxError& e) {
                    TME_ASSERT(false, "{}: {}", e.type(), e.what());
                } CATCH_ALL

                m_va = va->getId();
                m_ib = ib->getId();
                m_tva = tva->getId();
            }
            
            ~App() {
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
