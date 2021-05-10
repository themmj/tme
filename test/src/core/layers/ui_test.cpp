#include "gtest/gtest.h"
#include "core/layers/ui.hpp"

namespace tme {
    namespace core {
        namespace layers {

            class _CounterUi : public Ui {
                public:
                uint32_t m_counter = 0;

                _CounterUi() : Ui() {}

                protected:
                void update() override {
                    ++m_counter;
                }
            };

            TEST(TestUiLayers, WindowUpdateReaction) {
                // update function should only trigger
                // on WindowUpdate event
                events::WindowUpdate wu(2.0);
                events::WindowClose wc;
                _CounterUi layer;

                EXPECT_EQ(layer.m_counter, 0);

                layer.onEvent(wc);
                EXPECT_EQ(layer.m_counter, 0);

                layer.onEvent(wu);
                EXPECT_EQ(layer.m_counter, 1);
            }

        }
    }
}
