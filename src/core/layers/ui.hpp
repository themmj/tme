#ifndef _CORE_LAYERS_UI_H
#define _CORE_LAYERS_UI_H
/** @file */

#include "core/events/event.hpp"
#include "core/events/mouse.hpp"
#include "core/events/window.hpp"
#include "core/layers/layer.hpp"
#include "core/events/dispatcher.hpp"

namespace tme {
    namespace core {
        namespace layers {

            /**//**
             * Layer specialization for Ui elements.
             *
             * Classes should be derived from this implementing their individual
             * layout by overriding the udpate function. The toString method can also
             * be overridden to get more detailed logs.
             */
            class Ui : public Layer, public events::Dispatcher<Ui> {
                public:
                /// Construct an Imgui layer
                Ui(const std::string& name = "Ui") : Layer(name), Dispatcher(this) {}
                virtual ~Ui() {}

                void onEvent(events::Event& event) override;

                protected:
                /**//**
                 * Update function to be used for individual layer design.
                 *
                 * This method is called for every WindowUpdate event issued
                 * by the Window.
                 */
                virtual void update() = 0;

                private:
                bool handleWindowUpdate(events::WindowUpdate& event);
            };

        }
    }
}

#endif
