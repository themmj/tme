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

        Defaults::Container::Container() : m_data() {}

        void Defaults::Container::add(core::Identifier id) {
            m_data.push_back(id);
        }
        bool Defaults::Container::has(core::Identifier id) const {
            return std::find(m_data.begin(), m_data.end(), id) != m_data.end();
        }


        core::Handle<Defaults> Defaults::instance() {
            static auto defaults = core::Handle<Defaults>(new Defaults());
            return defaults;
        }

        void Defaults::cleanUp() {
            for (auto shaderId : shaders)  {
                core::Storage<core::graphics::Shader>::global()->destroy(shaderId); 
            }
            for (auto textureId : textures)  {
                core::Storage<core::graphics::Texture>::global()->destroy(textureId); 
            }
        }

        Defaults::Defaults() : shaders(), textures() {}


        Tilemap::Tilemap(uint32_t width, uint32_t height, uint32_t tileSize)
            : m_tileSize(tileSize),
            m_width(width),
            m_height(height),
            m_shaderIds(),
            m_textureIds(),
            m_cursor(new Cursor()),
            m_background(nullptr) {
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

        void Tilemap::addShader(core::Identifier shaderId) {
             m_shaderIds.push_back(shaderId);
        }

        void Tilemap::addTexture(core::Identifier textureId) {
             m_textureIds.push_back(textureId);
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

