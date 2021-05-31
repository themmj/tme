/** @file */
#include "app/graphics/texture.hpp"
#include <sstream>
#include "core/storage.hpp"
#include "core/graphics/shader.hpp"
#include "core/graphics/texture.hpp"
#include "core/exceptions/input.hpp"

namespace tme {
    namespace app {
        namespace graphics {

            TextureTile::TextureTile(core::Identifier id, uint32_t x, uint32_t y, core::Identifier shaderId, core::Identifier textureId, const Frames& frames)
                : Tile(id, x, y, shaderId),
                  m_textureId(textureId),
                  m_frames(frames),
                  m_activeFrame(0) {
                if (frames.size() < 1) {
                   throw core::exceptions::InvalidInput("No frames for texture tile provided");
                }
                float xf = static_cast<float>(x);
                float yf = static_cast<float>(y);
                Frame frame = m_frames[m_activeFrame];
                m_verticies[0] = {{0.0f + xf, 0.0f + yf}, {frame.texPos.x, frame.texPos.y}};
                m_verticies[1] = {{1.0f + xf, 0.0f + yf}, {frame.texPos.z, frame.texPos.y}};
                m_verticies[2] = {{0.0f + xf, 1.0f + yf}, {frame.texPos.x, frame.texPos.w}};
                m_verticies[3] = {{1.0f + xf, 1.0f + yf}, {frame.texPos.z, frame.texPos.w}};
            }

            core::Identifier TextureTile::createDefaultShader() {
                auto globalShaderStages = core::Storage<core::graphics::Shader::Stage>::global();
                auto globalShaders = core::Storage<core::graphics::Shader>::global();
                using StageType = core::graphics::Shader::Stage::Type;
                static core::Identifier shaderId = globalShaders->create(
                        globalShaderStages->create(StageType::Vertex, "texture", "../res/vertex-texture.glsl"),
                        globalShaderStages->create(StageType::Fragment, "texture", "../res/fragment-texture.glsl")
                        )->getId();
                 return shaderId;
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


            TextureTileFactory::TextureTileFactory(core::Identifier textureId)
                : TileFactory(TextureTile::createDefaultShader()), m_frames(), m_textureId(textureId) {}

            TextureTileFactory::~TextureTileFactory() {}

            Tile* TextureTileFactory::construct() {
                return new TextureTile(generateId(), m_x, m_y, m_shaderId, m_textureId, m_frames);
            }

            void TextureTileFactory::setTexture(core::Identifier textureId) {
                m_textureId = textureId;
                m_frames.clear();
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

