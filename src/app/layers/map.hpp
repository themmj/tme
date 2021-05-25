#ifndef _APP_LAYERS_MAP_H
#define _APP_LAYERS_MAP_H
/** @file */

#include "core/events/event.hpp"
#include "core/events/dispatcher.hpp"
#include "core/events/window.hpp"
#include "core/graphics/batch.hpp"
#include "core/layers/layer.hpp"
#include "core/storage.hpp"
#include "app/tilemap.hpp"

namespace tme {
    namespace app {
        namespace layers {

            /**//**
             * \brief Tilemap Layer handling the creation and deletion of tiles.
             *
             * Uses the Cursor of the Tilemap to determine if it should add/remove tiles every frame.
             * Additionally updates all tiles with the delta time of the WindowUpdate.
             */
            class MapLayer final : public core::layers::Layer, public core::events::Dispatcher<MapLayer> {
                size_t m_layerNumber;
                core::Handle<Cursor> m_cursor;
                core::graphics::Batcher m_batcher;
                core::Handle<core::Storage<graphics::Tile>> m_tiles;

                public:
                /**//**
                 * \brief Construct MapLayer of a Tilemap.
                 *
                 * @param layerNumber the number of the layer, layers with higher numbers are on top of lower numbers
                 * @param size how many tiles the map layer should be able to contain
                 * @param cursor Handle to the Cursor of the Tilemap that should be edited with this layer
                 */
                MapLayer(size_t layerNumber, size_t size, core::Handle<Cursor> cursor);
                ~MapLayer();

                void render() override;

                void onEvent(core::events::Event& event) override;

                private:
                bool handleWindowUpdate(core::events::WindowUpdate& event);
            };

        }
    }
}

#endif

