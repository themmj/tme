/** @file */

#include "app/layers/map.hpp"
#include "core/events/window.hpp"
#include "core/layers/layer.hpp"
#include "core/key.hpp"
#include "core/storage.hpp"

namespace tme {
    namespace app {
        namespace layers {

            MapLayer::MapLayer(size_t layerNumber, size_t size, core::Handle<Cursor> cursor)
                : core::layers::Layer("MapLayer"),
                Dispatcher(this),
                m_layerNumber(layerNumber),
                m_cursor(cursor),
                m_batcher(size) {
                m_tiles = core::Storage<graphics::Tile>::localInstance();
            }
            MapLayer::~MapLayer() {}

            void MapLayer::render() {
                m_batcher.render();
            }

            void MapLayer::onEvent(core::events::Event& event) {
                dispatchEvent<core::events::WindowUpdate>(event, &MapLayer::handleWindowUpdate);
            }

            bool MapLayer::handleWindowUpdate(core::events::WindowUpdate& event) {
                // update tiles
                for (auto iter : *m_tiles) {
                    if (iter.second->update(event.getDeltaTime())) {
                        m_batcher.set(iter.second);
                    }
                }

                // place/erase operations
                if (m_layerNumber != m_cursor->layer) {
                    return false;
                }
                if (m_cursor->inBounds && m_cursor->placeTile) {
                    if (!m_tiles->has(m_cursor->tileFactory->generateId())) {
                        auto tile = m_tiles->add(m_cursor->tileFactory->construct());
                        m_batcher.set(tile);
                    }
                    return true;
                }
                if (m_cursor->inBounds && m_cursor->eraseTile) {
                    core::Identifier tileId = m_cursor->tileFactory->generateId();
                    if (m_tiles->has(tileId)) {
                        m_batcher.unset(m_tiles->get(tileId));
                        m_tiles->destroy(tileId);
                    }
                    return true;
                }
                return false;
            }

        }
    }
}

