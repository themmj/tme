#ifndef _CORE_APPLICATION_H
#define _CORE_APPLICATION_H
/** @file */

#include "core/events/dispatcher.hpp"
#include "core/events/window.hpp"
#include "core/graphics/common.hpp"
#include "core/layers/layer.hpp"
#include "core/storage.hpp"
#include "core/window.hpp"
#include <string>

namespace tme {
    namespace core {

        /**//**
         * \brief Base class for applications.
         *
         * Provides functionality to use it inside storages.
         * Virtual run method for custom execution logic.
         */
        class Application : public Mappable {
            private:
            Identifier m_id;
        
            public:
            /**//**
             * \brief Default constructor.
             */
            Application();
            virtual ~Application();

            Identifier getId() const override { return m_id; }

            /**//**
             * \brief Virtual method to be overwritten by implementations.
             *
             * This allows for custom execution logic.
             */
            virtual void run() = 0;
        };

        /**//**
         * \brief Application implementation which creates a window.
         *
         * Handles creating a window and corresponding layer stack.
         * Implements the run method and calls the Renderable render function every frame.
         */
        class WindowApplication : public Application, public events::Dispatcher<WindowApplication>, public graphics::Renderable {
            protected:
            /// identifier of the created window
            Identifier m_window;
            /// layers::Stack on which different Layer implementations can be pushed
            layers::Stack m_layers;

            private:
            bool m_running;
        
            public:
            /**//**
             * \brief Construct WindowApplication instance.
             *
             * @param name the name of the app, which is also shows in the window title bar
             */
            WindowApplication(const std::string& name);
            virtual ~WindowApplication();

            /**//**
             * \brief Run implementation containing the update/render loop.
             */
            void run() override;

            void onEvent(events::Event& event) override;

            private:
            bool handleWindowClose(events::WindowClose&);
        };

    }
}

#endif

