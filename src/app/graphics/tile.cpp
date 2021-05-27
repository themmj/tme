/** @file */
#include "app/graphics/tile.hpp"
#include <sstream>
#include "core/storage.hpp"
#include "core/graphics/batch.hpp"
#include "core/graphics/shader.hpp"

namespace tme {
    namespace app {
        namespace graphics {

            Tile::Tile(core::Identifier id, uint32_t x, uint32_t y, core::Identifier shaderId)
                : m_indices(0),
                m_id(id),
                m_x(x),
                m_y(y),
                m_shaderId(shaderId),
                m_clock(0) {}

            Tile::~Tile() {}

            bool Tile::update(double deltaTime) {
                m_clock += deltaTime;
                return false;
            }

            core::Identifier Tile::getId() const {
                return m_id;
            }

            void Tile::setIndexOffset(unsigned int indexOffset) {
                m_indices = Index(indexOffset);
            }

            const void* Tile::getIndexData() const {
                return &m_indices;
            }

            const void* Tile::getVertexData() const {
                return NULL;
            }

            std::string Tile::toString() const {
                std::stringstream ss;
                ss << "Tile(" << m_x << ',' << m_y << ',' << m_shaderId << ')';
                return ss.str();
            }

            core::graphics::Batch::Config::Index Tile::s_indexConfig() {
                core::graphics::Batch::Config::Index indexData(1, sizeof(Tile::Index), 6);
                return indexData;
            }

            core::graphics::Batch::Config Tile::getBatchConfig() const {
                static auto vertexData = core::graphics::Batch::Config::Vertex(0, 0, 0);
                static auto indexData = Tile::s_indexConfig();
                return core::graphics::Batch::Config(vertexData, indexData, [](core::Identifier shaderId, core::Identifier){
                            if (auto shader = core::Storage<core::graphics::Shader>::global()->get(shaderId); shader) {
                                shader->bind();
                            }
                        }, m_shaderId, core::graphics::NO_TEXTURE);
            }


            TileFactory::TileFactory(core::Identifier shaderId)
                : m_x(0), m_y(0), m_shaderId(shaderId) {}

            TileFactory::~TileFactory() {}

            core::Identifier TileFactory::generateId() const {
                return (m_x << 16) | m_y;
            }

            void TileFactory::setPosition(uint32_t x, uint32_t y) {
                m_x = x;
                m_y = y;
            }

            void TileFactory::setShader(core::Identifier shaderId) {
                m_shaderId = shaderId;
            }

        }
    }
}

