/** @file */

#include "app/tilemap.hpp"
#include "core/storage.hpp"
#include "core/exceptions/common.hpp"
#include "core/graphics/shader.hpp"
#include "core/graphics/texture.hpp"
#include "app/graphics/tile.hpp"
#include "app/graphics/color.hpp"
#include "app/graphics/texture.hpp"
#include "app/layers/background.hpp"
#include "app/layers/map.hpp"

namespace tme {
    namespace app {


        Tilemap::Tilemap(uint32_t width, uint32_t height, uint32_t tileSize)
            : m_tileSize(tileSize),
            m_width(width),
            m_height(height),
            m_cursor(new Cursor()),
            m_background(nullptr) {
            addLayer();
        }

        Tilemap::~Tilemap() {}

        core::Identifier Tilemap::getId() const {
            return m_id;
        }

        void Tilemap::onEvent(core::events::Event& e) {
            m_layers.onEvent(e);
        }

        void Tilemap::render() {
            if (m_background) {
                m_background->render();
            }
            m_layers.render();
        }

        void Tilemap::addLayer() {
            m_layers.push<layers::MapLayer>(m_layerCount++, m_width * m_height, m_cursor);
        }
        void Tilemap::removeLayer() {
            m_layerCount--;
            m_layers.pop();
        }

        void Tilemap::setBackground(glm::vec4 color) {
            m_background = core::Handle<layers::Background>(new layers::Background(m_width, m_height, color));
        }

    }
}

