/** @file */

#include "app/editor.hpp"
#include "app/layers/ui.hpp"
#include "core/layers/imgui.hpp"
#include "core/storage.hpp"
#include "app/layers/editing.hpp"

#include "app/graphics/tile.hpp"
#include "core/graphics/shader.hpp"
#include "core/graphics/texture.hpp"
#include "core/exceptions/input.hpp"
#include "core/exceptions/validation.hpp"

namespace tme {
    namespace app {

        Editor::Editor(core::Handle<Tilemap> tilemap)
            : WindowApplication("TME"),
            m_tilemap(tilemap) {

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            try {
                auto vertex = core::Storage<core::graphics::Shader::Stage>::global()->create(core::graphics::Shader::Stage::Type::Vertex, "color", "/home/mmj/development/workspaces/cpp/tme/res/vertex-color.glsl");
                auto fragment = core::Storage<core::graphics::Shader::Stage>::global()->create(core::graphics::Shader::Stage::Type::Fragment, "color", "/home/mmj/development/workspaces/cpp/tme/res/fragment-color.glsl");
                auto shader = core::Storage<core::graphics::Shader>::global()->create(vertex, fragment);
                auto tvertex = core::Storage<core::graphics::Shader::Stage>::global()->create(core::graphics::Shader::Stage::Type::Vertex, "textured", "/home/mmj/development/workspaces/cpp/tme/res/vertex-texture.glsl");
                auto tfragment = core::Storage<core::graphics::Shader::Stage>::global()->create(core::graphics::Shader::Stage::Type::Fragment, "textured", "/home/mmj/development/workspaces/cpp/tme/res/fragment-texture.glsl");
                auto tshader = core::Storage<core::graphics::Shader>::global()->create(tvertex, tfragment);
                auto texture = core::Storage<core::graphics::Texture>::global()->create("/home/mmj/development/workspaces/cpp/tme/res/gh-skin.png");
                m_tilemap->addShader(shader->getId());
                m_tilemap->addShader(tshader->getId());
                m_tilemap->addTexture(texture->getId());
            } catch(const core::exceptions::InvalidInput& e) {
                TME_ASSERT(false, "{}: {}", e.type(), e.what());
            } catch(const core::exceptions::SyntaxError& e) {
                TME_ASSERT(false, "{}: {}", e.type(), e.what());
            } CATCH_ALL

            m_layers.push<layers::Editing>(m_tilemap);
            m_layers.push<layers::EditingUI>(m_tilemap);
            m_layers.push<core::layers::Imgui>();
        }

        Editor::~Editor() {
            core::Storage<Tilemap>::global()->destroy(m_tilemap->getId());
        }

        void Editor::render() {
            glClearColor(0.2f, 0.5f, 0.2f, 1.0f );
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            m_layers.render();
        }
    }
}

