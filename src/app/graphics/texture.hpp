#ifndef _APP_GRAPHICS_TEXTURE_H
#define _APP_GRAPHICS_TEXTURE_H
/** @file */

#include "app/graphics/tile.hpp"
#include <vector>
#include "core/storage.hpp"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

namespace tme {
    namespace app {
        namespace graphics {

            /**//**
             * \brief Tile implementation for textured tiles.
             *
             * Should only be created with TextureTileFactory.
             * Allows for animations by adding frames.
             */
            class TextureTile final : public Tile {
                public:
                /**//**
                 * \brief Animation frame.
                 *
                 * A single frame of animation.
                 * The texture coordinates are in the normalised format
                 * xMin, yMin, xMax, yMax. The first two elements indicate the bottom left corner
                 * of the texture and the other two indice the top right corner.
                 */
                struct Frame {
                    /// duration of the frame in seconds
                    double time;
                    /// normalised coordinates on the sprite sheet
                    glm::vec4 texPos;
                };
                /// type alias for a vector of frames
                using Frames = std::vector<Frame>;

                private:
                struct Vertex {
                    glm::vec2 pos, texPos;
                };

                Vertex m_verticies[4];
                core::Identifier m_textureId;
                Frames m_frames;
                size_t m_activeFrame;

                static core::graphics::Batch::Config::Vertex s_vertexConfig();

                public:
                /**//**
                 * \brief Construct TextureTile.
                 *
                 * Creates a tile for a specific texture with >= 1 frame.
                 *
                 * @param id identifier of the tile, should be provided by ColorTileFactory
                 * @param x position of the tile on x axis in full tiles
                 * @param y position of the tile on y axis in full tiles
                 * @param shaderId global Identifier of the Shader to be used
                 * @param textureId global Identifier of the Texture to be used
                 * @param frames frames of animation to be used, has to contain at least one
                 */
                TextureTile(core::Identifier id, uint32_t x, uint32_t y, core::Identifier shaderId, core::Identifier textureId, const Frames& frames);
                ~TextureTile() = default;

                /**//**
                 * \brief Get global Identifier of a default shader for the tile.
                 *
                 * @return global Shader Identifier 
                 */
                static core::Identifier s_defaultShaderId();

                /**//**
                 * \brief Update current frame.
                 *
                 * Uses Tile::update to update the internal clock.
                 * Then updates the frame if the passed time has exceeded the duration of the frame.
                 *
                 * @param deltaTime time since the last update in seconds
                 *
                 * @return true if the frame has been updated and requires a rerender, false otherwise
                 */
                bool update(double deltaTime) override;

                core::graphics::Batch::Config getBatchConfig() const override;
                const void* getVertexData() const override;

                std::string toString() const override;
            };


            /**//**
             * \brief TileFactory implementation for textured tiles.
             *
             * Additionally stores texture and animation frames.
             */
            class TextureTileFactory final : public TileFactory {
                TextureTile::Frames m_frames;
                core::Identifier m_textureId;

                public:
                /**//**
                 * \brief Construct TextureTileFactory with shader and texture.
                 *
                 * Provides the TextureTile::s_defaultShaderId() to the base TileFactory.
                 *
                 * @param textureId global id to the initial Texture to be used for the tiles
                 */
                TextureTileFactory(core::Identifier textureId);
                ~TextureTileFactory();

                Tile* construct() override;

                /**//**
                 * \brief Update associated texture.
                 *
                 * @param textureId global id of the new Texture to be used for future tiles
                 */
                void setTexture(core::Identifier textureId);
                /**//**
                 * \brief Get global id of associated Texture.
                 *
                 * @return global id of the Texture used for future tiles
                 */
                inline core::Identifier getTexture() const { return m_textureId; }

                /**//**
                 * \brief Get frames used for future tiles.
                 *
                 * @return reference to vector of animation frames
                 */
                inline const std::vector<TextureTile::Frame>& getFrames() const { return m_frames; }
                /**//**
                 * \brief Add a Frame of animation.
                 *
                 * @param frame the Frame to be added to future tiles' frames
                 */
                void addFrame(const TextureTile::Frame& frame);
                /**//**
                 * \brief Remove a Frame of animation.
                 *
                 * @param frameIndex index of the Frame to be removed
                 */
                void removeFrame(int64_t frameIndex);
                /**//**
                 * \brief Removes all frames.
                 */
                void clearFrames();
            };

        }
    }
}

#endif

