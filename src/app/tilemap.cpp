/** @file */

#include "app/tilemap.hpp"
#include "core/graphics/shader.hpp"
#include "core/graphics/texture.hpp"
#include "core/storage.hpp"
#include "app/layers/map.hpp"

namespace tme {
    namespace app {

        Tilemap::Tilemap(uint32_t width, uint32_t height, uint32_t tileSize)
            : m_tileSize(tileSize),
            m_width(width),
            m_height(height),
            m_shaderIds(),
            m_textureIds(),
            m_cursor(new Cursor()) {
            addLayer();
        }

        Tilemap::~Tilemap() {
            for (auto shaderId : m_shaderIds) {
                core::Storage<core::graphics::Shader>::global()->destroy(shaderId);
            }
            for (auto textureId : m_textureIds) {
                core::Storage<core::graphics::Texture>::global()->destroy(textureId);
            }
        }

        void Tilemap::onEvent(core::events::Event& e) {
            m_layers.onEvent(e);
        }

        void Tilemap::render() {
            m_layers.render();
        }

        void Tilemap::addLayer() {
            m_layers.push<layers::MapLayer>(m_layerCount++, m_width * m_height, m_cursor);
        }
        void Tilemap::removeLayer() {
            m_layerCount--;
            m_layers.pop();
        }

    }
}

