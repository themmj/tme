#include "gtest/gtest.h"
#include "core/events/window.hpp"
#include "core/layers/imgui.hpp"
#include "core/window.hpp"
#include "platform/context.hpp"

namespace tme {
    namespace core {
        namespace layers {

            TEST(TestImguiLayers, CreationOnWindow) {
                auto context = platform::Context::create();
                auto window = Window::create({nullptr, "This is a test", 640, 300, true});
                events::WindowUpdate wu(2.0);
                Imgui layer;

                layer.onEvent(wu);

                delete window;
            }

        }
    }
}
