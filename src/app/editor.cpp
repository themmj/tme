/** @file */

#include "app/editor.hpp"
#include "core/graphics/common.hpp"
#include "core/layers/imgui.hpp"
#include "core/layers/layer.hpp"
#include "core/storage.hpp"
#include "app/layers/ui.hpp"
#include "app/layers/editing.hpp"
#include "app/layers/menu.hpp"

namespace tme {
    namespace app {

        Editor::Editor(core::Handle<Tilemap> tilemap) : WindowApplication("TME") {
            setTilemap(tilemap);
        }

        Editor::~Editor() {
            core::Storage<Tilemap>::global()->clear();
            core::graphics::cleanUp();
        }

        void Editor::render() {
            glClearColor(0.2f, 0.2f, 0.3f, 1.0f );
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            if (m_reconstruct) {
                constructLayers();
            }
            m_layers.render();
        }

        void Editor::setTilemap(core::Handle<Tilemap> tilemap) {
            m_tilemap = tilemap;
            m_reconstruct = true;
        }

        void Editor::constructLayers() {
            while (m_layers.pop());
            if (m_tilemap) {
                m_layers.push<layers::Editing>(m_tilemap, core::Storage<core::Window>::global()->get(m_window));
                m_layers.push<layers::EditingUI>(m_tilemap);
            }
            m_layers.push<layers::MenuBar>(getId());
            m_layers.push<core::layers::Imgui>();
            m_reconstruct = false;
        }
    }
}

