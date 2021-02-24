#ifndef _CORE_LAYERS_IMGUI_H
#define _CORE_LAYERS_IMGUI_H
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
             * Layer specialization for ImGui.
             *
             * This layer handles input and resizing events and forwards them
             * to ImGui.
             * Classes should be derived from this implementing their individual
             * layout by overriding the udpate function. The toString method can also
             * be overridden to get more detailed logs.
             */
            class Imgui : public Layer, public events::Dispatcher<Imgui> {
                static bool s_initialized;
                public:
                /// Construct an Imgui layer
                Imgui(const std::string& name = "Imgui") : Layer(name), Dispatcher(this) {}
                ~Imgui() {}

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
                bool handleKeyPress(events::KeyPress& event);
                bool handleKeyRelease(events::KeyRelease& event);
                bool handleKeyChar(events::KeyChar& event);
                bool handleMouseKeyPress(events::MouseKeyPress& event);
                bool handleMouseKeyRelease(events::MouseKeyRelease& event);
                bool handleMouseMove(events::MouseMove& event);
                bool handleMouseScroll(events::MouseScroll& event);
                bool handleWindowUpdate(events::WindowUpdate& event);
                bool handleWindowResize(events::WindowResize& event);
            };

            /// Derived class of Imgui showing the ImGui demo window
            class DemoImgui : public Imgui {
                public:
                DemoImgui() : Imgui() {}
                
                protected:
                void update() override;
            };

        }
    }
}

#endif
