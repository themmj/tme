#ifndef _CORE_APPLICATION_H
#define _CORE_APPLICATION_H
/** @file */

#include "core/events/dispatcher.hpp"
#include "core/events/window.hpp"
#include "core/layers/layer.hpp"
#include "core/map.hpp"
#include "core/window.hpp"
#include <string>

namespace tme {
    namespace core {

        class Application : public Mappable, events::Dispatcher<Application> {
            protected:
            Identifier m_window;
            layers::Stack m_layers;

            private:
            bool m_running;
            Identifier m_id;
        
            public:
            Application(const std::string& name);
            virtual ~Application();

            inline bool isRunning() const { return m_running; }
            void update();

            Identifier getId() const override { return m_id; }

            void onEvent(events::Event& event) override;
            bool handleWindowClose(events::WindowClose&);


            protected:
            virtual void updateInternal() = 0;
        };

    }
}

#endif

