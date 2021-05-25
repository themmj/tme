#ifndef _APP_GRAPHICS_TILE_H
#define _APP_GRAPHICS_TILE_H
/** @file */

#include <vector>
#include "core/storage.hpp"
#include "core/graphics/batch.hpp"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

namespace tme {
    namespace app {
        namespace graphics {
            /**//**
             * \brief Base class for tiles.
             *
             * All tiles should derive from this and provide a factory implementation to be usable with
             * the editor.
             * @sa TileFactory
             * Implements a default index, providing access to its definition, update functionality
             * and data access.
             */
            class Tile : public core::Loggable, public core::graphics::Batchable {
                protected:
                /**//**
                 * \brief Default index structure of a Tile.
                 *
                 * Consists of two triangles forming a square.
                 */
                struct Index {
                    /**//**
                     * \brief A single triangle.
                     */
                    struct Triangle {
                        /// first vertex index
                        unsigned int first;
                        /// first vertex index
                        unsigned int second;
                        /// first vertex index
                        unsigned int third;

                        /**//**
                         * \brief Construct Triangle from offset.
                         *
                         * For an offset of 3 the resulting Triangle contains 3, 4, 5.
                         *
                         * @param offset how much the 3 primitives should be offset
                         */
                        explicit Triangle(unsigned int offset)
                        : first(0 + offset),
                          second(1 + offset),
                          third(2 + offset) {}
                    };

                    /// first triangle
                    Triangle t1;
                    /// second triangle
                    Triangle t2;

                    /**//**
                     * \brief Construct Index from offset.
                     *
                     * As a square only uses four vertices the Triangles of the index will
                     * reuse two vertices.
                     * For an offset of 3 the resulting triangles would contain:
                     * 3, 4, 5 and
                     * 4, 5, 6.
                     *
                     * @param offset how much the initial vertex is offset from index 0
                     */
                    explicit Index(unsigned int offset): t1(offset), t2(offset + 1) {}
                };

                /// index value object
                Index m_indices;
                /// identifier of the tile
                core::Identifier m_id;
                /// x position of the tile in full tiles
                uint32_t m_x;
                /// y position of the tile in full tiles
                uint32_t m_y;
                /// global Identifier of the shader that should be used for the tile
                core::Identifier m_shaderId;
                /// allows to keep track of time in seconds, is updated by update()
                double m_clock;

                public:
                /**//**
                 * \brief Construct Tile instance.
                 *
                 * @param id identifier of the tile, should be provided by TileFactory
                 * @param x position of the tile on x axis in full tiles
                 * @param y position of the tile on y axis in full tiles
                 * @param shaderId global Identifier of the Shader to be used
                 */
                Tile(core::Identifier id, uint32_t x, uint32_t y, core::Identifier shaderId);
                virtual ~Tile();

                core::Identifier getId() const override;

                /**//**
                 * \brief Updates internal clock.
                 *
                 * Can be overloaded by derived classes to implement i.e. animations.
                 *
                 * @param deltaTime amount of time passed since the last update in seconds
                 *
                 * @return true if the Tile should be rerendered, false if nothing has changed
                 */
                virtual bool update(double deltaTime);

                virtual core::graphics::Batch::Config getBatchConfig() const override;
                virtual void setIndexOffset(unsigned int indexOffset) override;
                virtual const void* getIndexData() const override;
                virtual const void* getVertexData() const override;

                virtual std::string toString() const override;

                protected:
                /**//**
                 * \brief Provide Batch::Config::Index for default index structure.
                 *
                 * @return index description to be used in a batch config
                 */
                static core::graphics::Batch::Config::Index s_indexConfig();
            };

            /**//**
             * \brief Tile implementation for colored tiles.
             *
             * Should only be created with ColorTileFactory.
             * All four vertices will have the same color.
             */
            class ColorTile final : public Tile {
                struct Vertex {
                    glm::vec2 pos;
                    glm::vec4 color;
                };

                Vertex m_verticies[4];

                static core::graphics::Batch::Config::Vertex s_vertexConfig();

                public:
                /**//**
                 * \brief Construct ColorTile.
                 *
                 * Stores the color for every tile but sets all to the same color.
                 * Could be used as a starting point to create a Tile with different colors on every vertex.
                 *
                 * @param id identifier of the tile, should be provided by ColorTileFactory
                 * @param x position of the tile on x axis in full tiles
                 * @param y position of the tile on y axis in full tiles
                 * @param shaderId global Identifier of the Shader to be used
                 * @param color color of all four vertices
                 */
                ColorTile(core::Identifier id, uint32_t x, uint32_t y, core::Identifier shaderId, glm::vec4 color);
                ~ColorTile() = default;

                core::graphics::Batch::Config getBatchConfig() const override;
                const void* getVertexData() const override;

                std::string toString() const override;
            };

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
             * \brief Base factory class to create tiles.
             *
             * Factories for the other tile types should derive from this to be used inside the editor.
             * Stores position and shader storage.
             * Provides the generation for an tile id based on the tile position.
             */
            class TileFactory {
                protected:
                /// x position of the to be created tile
                uint32_t m_x;
                /// y position of the to be created tile
                uint32_t m_y;
                /// associated shader
                core::Identifier m_shaderId;

                public:
                /**//**
                 * \brief Construct TileFactory with shader.
                 *
                 * @param shaderId global id to the initial shader to be used for the tiles
                 */
                TileFactory(core::Identifier shaderId);
                virtual ~TileFactory();

                /**//**
                 * \brief Construct Tile from current state of the factory.
                 *
                 * @return owning pointer to newly created Tile
                 */
                virtual Tile* construct() = 0;

                /**//**
                 * \brief Generate id of a tile based on current state.
                 *
                 * @return id of the to be created Tile
                 */
                core::Identifier generateId() const;

                /**//**
                 * \brief Update position of the to be created Tile.
                 *
                 * @param x position in x direction in full tiles
                 * @param y position in y direction in full tiles
                 */
                void setPosition(uint32_t x, uint32_t y);

                /**//**
                 * \brief Update associated Shader.
                 *
                 * @param shaderId global id of the new Shader to be used for future tiles
                 */
                void setShader(core::Identifier shaderId);
            };

            /**//**
             * \brief TileFactory implementation for colored tiles.
             *
             * Additionally stores color.
             */
            class ColorTileFactory final : public TileFactory {
                glm::vec4 m_color;

                public:
                /**//**
                 * \brief Construct ColorTileFactory with shader.
                 *
                 * @param shaderId global id to the initial Shader to be used for the tiles
                 */
                ColorTileFactory(core::Identifier shaderId);
                ~ColorTileFactory();

                Tile* construct() override;

                /**//**
                 * \brief Update associated color.
                 *
                 * @param color color to be used for future tiles
                 */
                void setColor(glm::vec4 color);
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
                 * @param shaderId global id to the initial Shader to be used for the tiles
                 * @param textureId global id to the initial Texture to be used for the tiles
                 */
                TextureTileFactory(core::Identifier shaderId, core::Identifier textureId);
                ~TextureTileFactory();

                Tile* construct() override;

                /**//**
                 * \brief Update associated texture.
                 *
                 * @param textureId global id of the new Texture to be used for future tiles
                 */
                void setTexture(core::Identifier textureId);
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

