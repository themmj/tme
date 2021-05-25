/** @file */
#include "app/graphics/tile.hpp"
#include <sstream>
#include "core/storage.hpp"
#include "core/graphics/batch.hpp"
#include "core/graphics/vertex.hpp"
#include "core/graphics/shader.hpp"
#include "core/graphics/texture.hpp"

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
                return &core::graphics::NULL_BYTE;
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


            ColorTile::ColorTile(core::Identifier id, uint32_t x, uint32_t y, core::Identifier shaderId, glm::vec4 color)
                : Tile(id, x, y, shaderId) {
                float xf = static_cast<float>(x);
                float yf = static_cast<float>(y);
                m_verticies[0] = {{0.0f + xf, 0.0f + yf}, {color.r, color.g, color.b, color.a}};
                m_verticies[1] = {{1.0f + xf, 0.0f + yf}, {color.r, color.g, color.b, color.a}};
                m_verticies[2] = {{0.0f + xf, 1.0f + yf}, {color.r, color.g, color.b, color.a}};
                m_verticies[3] = {{1.0f + xf, 1.0f + yf}, {color.r, color.g, color.b, color.a}};
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


            TextureTile::TextureTile(core::Identifier id, uint32_t x, uint32_t y, core::Identifier shaderId, core::Identifier textureId, const Frames& frames)
                : Tile(id, x, y, shaderId),
                  m_textureId(textureId),
                  m_frames(frames),
                  m_activeFrame(0) {
                TME_ASSERT(frames.size() >= 1, "no frames for texture tile provided");
                float xf = static_cast<float>(x);
                float yf = static_cast<float>(y);
                Frame frame = m_frames[m_activeFrame];
                m_verticies[0] = {{0.0f + xf, 0.0f + yf}, {frame.texPos.x, frame.texPos.y}};
                m_verticies[1] = {{1.0f + xf, 0.0f + yf}, {frame.texPos.z, frame.texPos.y}};
                m_verticies[2] = {{0.0f + xf, 1.0f + yf}, {frame.texPos.x, frame.texPos.w}};
                m_verticies[3] = {{1.0f + xf, 1.0f + yf}, {frame.texPos.z, frame.texPos.w}};
            }

            bool TextureTile::update(double deltaTime) {
                Tile::update(deltaTime);
                if (m_frames.size() < 2) {
                    return false;
                }
                Frame currentFrame = m_frames[m_activeFrame];
                if (m_clock < currentFrame.time) {
                    return false;
                }
                m_clock -= currentFrame.time;
                m_activeFrame = (m_activeFrame + 1) % m_frames.size();
                Frame nextFrame = m_frames[m_activeFrame];
                m_verticies[0].texPos = {nextFrame.texPos.x, nextFrame.texPos.y};
                m_verticies[1].texPos = {nextFrame.texPos.z, nextFrame.texPos.y};
                m_verticies[2].texPos = {nextFrame.texPos.x, nextFrame.texPos.w};
                m_verticies[3].texPos = {nextFrame.texPos.z, nextFrame.texPos.w};
                return true;
            }

            core::graphics::Batch::Config::Vertex TextureTile::s_vertexConfig() {
                auto vl = core::Storage<core::graphics::VertexLayout>::global()->create();
                vl->push<float>(2);
                vl->push<float>(2);
                core::graphics::Batch::Config::Vertex vertexData(4, sizeof(Vertex), vl->getId());
                return vertexData;
            }

            core::graphics::Batch::Config TextureTile::getBatchConfig() const {
                static auto vertexData = TextureTile::s_vertexConfig();
                auto config = Tile::getBatchConfig();
                config.vertex = vertexData;
                config.textureId = m_textureId;
                config.preRender = [](core::Identifier shaderId, core::Identifier textureId){
                    if (auto shader = core::Storage<core::graphics::Shader>::global()->get(shaderId); shader) {
                        shader->bind();
                        if (auto texture = core::Storage<core::graphics::Texture>::global()->get(textureId); texture) {
                            texture->bind();
                            shader->setUniform1i("u_texture", static_cast<int>(texture->getSlot()));
                        }
                    }
                };
                return config;
            }
            const void* TextureTile::getVertexData() const {
                return &m_verticies;
            }

            std::string TextureTile::toString() const {
                std::stringstream ss;
                ss << "TextureTile(" << m_x << ',' << m_y << ',' << m_shaderId << ',' << m_textureId << ')';
                return ss.str();
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


            ColorTileFactory::ColorTileFactory(core::Identifier shaderId)
                : TileFactory(shaderId), m_color(0.0, 0.0, 0.0, 0.0) {}

            ColorTileFactory::~ColorTileFactory() {}

            Tile* ColorTileFactory::construct() {
                return new ColorTile(generateId(), m_x, m_y, m_shaderId, m_color);
            }

            void ColorTileFactory::setColor(glm::vec4 color) {
                m_color = color;
            }


            TextureTileFactory::TextureTileFactory(core::Identifier shaderId, core::Identifier textureId)
                : TileFactory(shaderId), m_frames(), m_textureId(textureId) {}

            TextureTileFactory::~TextureTileFactory() {}

            Tile* TextureTileFactory::construct() {
                return new TextureTile(generateId(), m_x, m_y, m_shaderId, m_textureId, m_frames);
            }

            void TextureTileFactory::setTexture(core::Identifier textureId) {
                m_textureId = textureId;
            }

            void TextureTileFactory::addFrame(const TextureTile::Frame& frame) {
                m_frames.push_back(frame);
            }

            void TextureTileFactory::removeFrame(int64_t frameIndex) {
                m_frames.erase(m_frames.begin() + frameIndex);
            }

            void TextureTileFactory::clearFrames() {
                m_frames.clear();
            }

        }
    }
}

