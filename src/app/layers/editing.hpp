#ifndef _APP_LAYERS_EDITING_H
#define _APP_LAYERS_EDITING_H
/** @file */

#include "core/storage.hpp"
#include "core/events/event.hpp"
#include "core/events/dispatcher.hpp"
#include "core/events/mouse.hpp"
#include "core/events/window.hpp"
#include "core/layers/layer.hpp"
#include "app/tilemap.hpp"
#include "app/camera.hpp"
#include "core/window.hpp"

namespace tme {
    namespace app {
        namespace layers {

            /**//**
             * \brief Application Layer handling the editing of a Tilemap.
             *
             * Handles control of the camera and updates the Cursor to make the multiple MapLayer of the TileMap
             * place/remove tiles.
             */
            class Editing final : public core::layers::Layer, public core::events::Dispatcher<Editing> {
                core::Handle<Tilemap> m_tilemap;
                Camera m_camera;
                bool m_cameraMoving;
                double m_prevCamX, m_prevCamY;

                public:
                /**//**
                 * \brief Construct Editing Layer for Tilemap.
                 *
                 * @param tilemap Handle to the Tilemap that should be edited with this layer
                 * @param window Handle to the Window in which the tilemap wil be edited
                 */
                Editing(core::Handle<Tilemap> tilemap, core::Handle<core::Window> window);
                ~Editing();

                void render() override;

                void onEvent(core::events::Event& event) override;

                private:
                bool handleWindowResize(core::events::WindowResize& event);
                bool handleMouseKeyPress(core::events::MouseKeyPress& event);
                bool handleMouseKeyReleased(core::events::MouseKeyRelease& event);
                bool handleMouseMove(core::events::MouseMove& event);
                bool handleMouseScroll(core::events::MouseScroll& event);
            };

        }
    }
}

#endif

