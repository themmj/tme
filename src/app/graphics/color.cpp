/** @file */
#include "app/graphics/color.hpp"
#include <sstream>
#include "core/storage.hpp"
#include "core/graphics/shader.hpp"

namespace tme {
    namespace app {
        namespace graphics {

            ColorTile::ColorTile(core::Identifier id, uint32_t x, uint32_t y, core::Identifier shaderId, glm::vec4 color)
                : Tile(id, x, y, shaderId) {
                float xf = static_cast<float>(x);
                float yf = static_cast<float>(y);
                m_verticies[0] = {{0.0f + xf, 0.0f + yf}, {color.r, color.g, color.b, color.a}};
                m_verticies[1] = {{1.0f + xf, 0.0f + yf}, {color.r, color.g, color.b, color.a}};
                m_verticies[2] = {{0.0f + xf, 1.0f + yf}, {color.r, color.g, color.b, color.a}};
                m_verticies[3] = {{1.0f + xf, 1.0f + yf}, {color.r, color.g, color.b, color.a}};
            }

            core::Identifier ColorTile::createDefaultShader() {
                auto globalShaderStages = core::Storage<core::graphics::Shader::Stage>::global();
                auto globalShaders = core::Storage<core::graphics::Shader>::global();
                using StageType = core::graphics::Shader::Stage::Type;
                static core::Identifier shaderId = globalShaders->create(
                        globalShaderStages->create(StageType::Vertex, "color", "../res/vertex-color.glsl"),
                        globalShaderStages->create(StageType::Fragment, "color", "../res/fragment-color.glsl")
                        )->getId();
                 return shaderId;
            }

            const void* ColorTile::getVertexData() const {
                return &m_verticies;
            }

            core::graphics::Batch::Config::Vertex ColorTile::s_vertexConfig() {
                auto vl = core::Storage<core::graphics::VertexLayout>::global()->create();
                vl->push<float>(2);
                vl->push<float>(4);
                core::graphics::Batch::Config::Vertex vertexData(4, sizeof(Vertex), vl->getId());
                return vertexData;
            }

            core::graphics::Batch::Config ColorTile::getBatchConfig() const {
                static auto vertexData = ColorTile::s_vertexConfig();
                auto config = Tile::getBatchConfig();
                config.vertex = vertexData;
                return config;
            }

            std::string ColorTile::toString() const {
                std::stringstream ss;
                ss << "ColorTile(" << m_x << ',' << m_y << ',' << m_shaderId << ')';
                return ss.str();
            }


            ColorTileFactory::ColorTileFactory()
                : TileFactory(ColorTile::createDefaultShader()), m_color(0.0, 0.0, 0.0, 0.0) {}

            ColorTileFactory::~ColorTileFactory() {}

            Tile* ColorTileFactory::construct() {
                return new ColorTile(generateId(), m_x, m_y, m_shaderId, m_color);
            }

            void ColorTileFactory::setColor(glm::vec4 color) {
                m_color = color;
            }

        }
    }
}

