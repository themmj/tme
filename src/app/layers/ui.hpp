#ifndef _APP_LAYERS_UI_H
#define _APP_LAYERS_UI_H
/** @file */

#include "app/graphics/tile.hpp"
#include "core/layers/layer.hpp"
#include "core/storage.hpp"
#include "app/tilemap.hpp"

namespace tme {
    namespace app {
        namespace layers {

            /**//**
             * \brief Layer implementation for providing editing features for a Tilemap.
             *
             * Uses the Cursor of the Tilemap to determine if it should add/remove tiles every frame.
             * Additionally updates all tiles with the delta time of the WindowUpdate.
             */
            class EditingUI final : public core::layers::Layer {
                core::Handle<Tilemap> m_tilemap;

                public:
                /**//**
                 * \brief Construct an EditingUI Layer for a Tilemap.
                 *
                 * @param tilemap Handle to the Tilemap that should be edited with this layer
                 */
                EditingUI(core::Handle<Tilemap> tilemap);
                ~EditingUI();

                void render() override;

                private:
                core::Handle<graphics::ColorTileFactory> m_colorTileFactory;
                core::Handle<graphics::TextureTileFactory> m_textureTileFactory;

                void showLayerSelection();
                void showTileSelection();

                void showColorTileSelection();
                void showTextureTileSelection();

                void showShaderSelection(core::Handle<graphics::TileFactory> factory);
                void showTextureSelection();

                bool m_errorOccurred;
                core::exceptions::Base m_error;
                void showErrors();
            };

        }
    }
}

#endif

