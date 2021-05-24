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
         * Base class for applications.
         *
         * Provides functionality to use it inside storages.
         * Virtual run method for custom execution logic.
         */
        class Application : public Mappable {
            private:
            Identifier m_id;
        
            public:
            /// default constructor
            Application();
            virtual ~Application();

            Identifier getId() const override { return m_id; }

            /// virtual method to be overwritten by implementations
            virtual void run() = 0;
        };

        /**//**
         * Application implementation which creates a window.
         *
         * Handles creating a window and corresponding layer stack.
         * Implements the run method and provides a virtual update method
         * which is called every frame.
         */
        class WindowApplication : public Application, public events::Dispatcher<WindowApplication>, public graphics::Renderable {
            protected:
            /// identifier of the created window
            /// can be passed to other components which acces it via Storage
            Identifier m_window;
            /// stack on which different Layer implementations can be pushed
            layers::Stack m_layers;

            private:
            bool m_running;
        
            public:
            /// construct WindowApplication with title name
            WindowApplication(const std::string& name);
            virtual ~WindowApplication();

            /// run implementation containing the update/render loop
            void run() override;

            void onEvent(events::Event& event) override;

            private:
            bool handleWindowClose(events::WindowClose&);
        };

    }
}

#endif

