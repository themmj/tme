/** @file */
#include "app/layers/background.hpp"
#include <sstream>
#include "app/graphics/color.hpp"

namespace tme {
    namespace app {
        namespace layers {

            Background::Background(uint32_t width, uint32_t height, glm::vec4 color)
                : Layer("Background"), m_batcher(width * height) {
                m_tiles = core::Storage<graphics::Tile>::localInstance();
                graphics::ColorTileFactory factory;
                factory.setColor(color);
                for (uint32_t x = 0; x < width; ++x) {
                    for (uint32_t y = 0; y < height; ++y) {
                        factory.setPosition(x, y);
                        auto tile = m_tiles->add(factory.construct());
                        m_batcher.set(tile);
                    }
                }
            }

            void Background::render() {
                m_batcher.render();
            }

        }
    }
}

