#ifndef _APP_LAYERS_BACKGROUND_H
#define _APP_LAYERS_BACKGROUND_H
/** @file */

#include "core/layers/layer.hpp"
#include "core/graphics/batch.hpp"
#include "app/graphics/tile.hpp"
#include "glm/vec4.hpp"

namespace tme {
    namespace app {
        namespace layers {

            /**//**
             * \brief Layer implementation for a single background rectangle.
             *
             * Allows to set a set area of the screen to a single color.
             * Fills entire area of a tilemap in one solid color using multiple ColorTile.
             */
            class Background final : public core::layers::Layer {
                core::graphics::Batcher m_batcher;
                core::Handle<core::Storage<graphics::Tile>> m_tiles;

                public:
                /**//**
                 * \brief Construct Background.
                 *
                 * Stores the color for every tile but sets all to the same color.
                 *
                 * @param width width in full tiles
                 * @param height height in full tiles
                 * @param color color of all four vertices
                 */
                Background(uint32_t width, uint32_t height, glm::vec4 color);
                ~Background() = default;

                void render() override;
            };

        }
    }
}

#endif

