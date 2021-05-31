/** @file */

#include "app/editor.hpp"
#include "core/graphics/common.hpp"
#include "core/layers/imgui.hpp"
#include "core/storage.hpp"
#include "app/layers/ui.hpp"
#include "app/layers/editing.hpp"

namespace tme {
    namespace app {

        Editor::Editor(core::Handle<Tilemap> tilemap)
            : WindowApplication("TME"),
            m_tilemap(tilemap) {

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            m_tilemap->setBackground({1.0f, 1.0f, 1.0f, 0.5f});

            m_layers.push<layers::Editing>(m_tilemap);
            m_layers.push<layers::EditingUI>(m_tilemap);
            m_layers.push<core::layers::Imgui>();
        }

        Editor::~Editor() {
            core::Storage<Tilemap>::global()->destroy(m_tilemap->getId());
            core::graphics::cleanUp();
        }

        void Editor::render() {
            glClearColor(0.2f, 0.2f, 0.3f, 1.0f );
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            m_layers.render();
        }
    }
}

