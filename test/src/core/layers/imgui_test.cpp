#include "gtest/gtest.h"
#include "core/events/window.hpp"
#include "core/layers/imgui.hpp"
#include "core/window.hpp"

namespace tme {
    namespace core {
        namespace layers {

            class _DefaultImgui : public Imgui {
                public:
                uint32_t m_counter = 0;

                _DefaultImgui() : Imgui() {}

                protected:
                void update() override {
                    ++m_counter;
                }
            };

            TEST(TestImguiLayers, WindowUpdateReaction) {
                // update function should only trigger
                // on WindowUpdate event
                auto window = Window::create({"This is a test", 640, 300, nullptr, true});
                events::WindowUpdate wu(2.0);
                events::WindowClose wc;

                _DefaultImgui layer;
                EXPECT_EQ(layer.m_counter, 0);

                layer.onEvent(wc);
                EXPECT_EQ(layer.m_counter, 0);

                layer.onEvent(wu);
                EXPECT_EQ(layer.m_counter, 1);
                delete window;
            }

            TEST(TestImguiLayers, CreateDemoImgui) {
                // verifies that classes overriding update can be instantiated
                DemoImgui layer;
            }

        }
    }
}
