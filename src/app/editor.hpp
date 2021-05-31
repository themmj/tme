#ifndef _APP_EDITOR_H
#define _APP_EDITOR_H
/** @file */

#include "core/application.hpp"
#include "core/storage.hpp"
#include "app/tilemap.hpp"

namespace tme {
    namespace app {
        /**//**
         * \brief Editor Application to edit a Tilemap.
         */
        class Editor : public core::WindowApplication {
            bool m_reconstruct = false;
            core::Handle<Tilemap> m_tilemap;

            public:
            /**//**
             * \brief Construct Editor for a tilemap.
             *
             * If no tilemap is provided it only creates a basic menu bar to
             * create a new Tilemap.
             *
             * @param tilemap Handle to a Tilemap to be edited by the Editor
             */
            Editor(core::Handle<Tilemap> tilemap = core::Handle<Tilemap>(nullptr));
            ~Editor();

            void render() override;

            /**//**
             * \brief Update the Tilemap to be edited.
             *
             * Causes layer Stack reconstruction on the next render call.
             *
             * @param tilemap Handle to the new Tilemap which should be edited
             */
            void setTilemap(core::Handle<Tilemap> tilemap);

            private:
            void constructLayers();
        };

    }
}

#endif

