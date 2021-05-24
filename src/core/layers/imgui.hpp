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
             * There should only be one Imgui layer because it updates imgui's
             * global state.
             * For the actual ui elements the Ui layer should be used.
             */
            class Imgui final : public Layer, public events::Dispatcher<Imgui> {
                public:
                /// Construct an Imgui layer
                Imgui() : Layer("Imgui"), Dispatcher(this) {}
                ~Imgui() {}

                void onEvent(events::Event& event) override;

                void render() override;

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

        }
    }
}

#endif
