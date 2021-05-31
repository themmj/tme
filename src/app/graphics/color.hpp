#ifndef _APP_GRAPHICS_COLOR_H
#define _APP_GRAPHICS_COLOR_H
/** @file */

#include "app/graphics/tile.hpp"
#include "core/storage.hpp"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

namespace tme {
    namespace app {
        namespace graphics {

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

                /**//**
                 * \brief Get global Identifier of a default shader for the tile.
                 *
                 * Creates the default shader once. Always returns its id from that point onwards.
                 *
                 * @return global Shader Identifier 
                 */
                static core::Identifier createDefaultShader();

                core::graphics::Batch::Config getBatchConfig() const override;
                const void* getVertexData() const override;

                std::string toString() const override;
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
                 * Provides the ColorTile::s_defaultShaderId() to the base TileFactory.
                 */
                ColorTileFactory();
                ~ColorTileFactory();

                Tile* construct() override;

                /**//**
                 * \brief Update associated color.
                 *
                 * @param color color to be used for future tiles
                 */
                void setColor(glm::vec4 color);
            };

        }
    }
}

#endif

