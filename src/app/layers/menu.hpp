#ifndef _APP_LAYERS_MENU_H
#define _APP_LAYERS_MENU_H
/** @file */

#include "core/layers/layer.hpp"
#include "core/storage.hpp"

namespace tme {
    namespace app {
        namespace layers {

            /**//**
             * \brief Layer implementation for the main menu bar.
             *
             * Allows to call back into the associated editor to update its Tilemap
             * once a new one has been created and other stuff in the future.
             */
            class MenuBar final : public core::layers::Layer {
                core::Identifier m_editorId;

                public:
                /**//**
                 * \brief Construct an MenuBar Layer for a Editor.
                 *
                 * @param editorId id of the editor associated with this layer
                 */
                MenuBar(core::Identifier editorId);
                ~MenuBar();

                void render() override;

                private:
                void showFileOptions();
            };

        }
    }
}

#endif

