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
            core::Handle<Tilemap> m_tilemap;

            public:
            /**//**
             * \brief Construct Editor for a tilemap.
             *
             * @param tilemap Handle to a Tilemap to be edited by the Editor
             */
            Editor(core::Handle<Tilemap> tilemap);
            ~Editor();

            void render() override;
        };

    }
}

#endif

