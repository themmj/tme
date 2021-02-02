#include "gtest/gtest.h"
#include "core/events/window.hpp"

namespace tme {
    namespace core {
        namespace events {

            TEST(EventTest, WindowClose) {
                WindowClose wc;

                EXPECT_EQ(wc.getType(), Type::WindowClose);
                EXPECT_TRUE(wc.isInCategory(Application));
                EXPECT_FALSE(wc.isInCategory(Mouse));
            }

            TEST(EventTest, WindowResize) {
                const uint32_t width = 220;
                const uint32_t height = 144;
                WindowResize wr(width, height);

                EXPECT_EQ(wr.getType(), Type::WindowResize);
                EXPECT_TRUE(wr.isInCategory(Application));
                EXPECT_FALSE(wr.isInCategory(Mouse));

                EXPECT_EQ(wr.getWidth(), width);
                EXPECT_EQ(wr.getHeight(), height);
                // custom to string
                EXPECT_EQ(wr.toString(), "WindowResize(220,144)");
            }

        }
    }
}
