#include "gtest/gtest.h"
#include "core/events/window.hpp"

namespace tme {
    namespace core {
        namespace events {

            TEST(EventTest, WindowUpdate) {
                double update = 2.5;
                WindowUpdate wu(update);

                EXPECT_EQ(wu.getType(), Type::WindowUpdate);
                EXPECT_TRUE(wu.isInCategory(Application));
                EXPECT_FALSE(wu.isInCategory(Keyboard));
                EXPECT_EQ(wu.getDeltaTime(), update);
                EXPECT_EQ(wu.toString(), "WindowUpdate(2.5)");
            }

            TEST(EventTest, WindowClose) {
                WindowClose wc;

                EXPECT_EQ(wc.getType(), Type::WindowClose);
                EXPECT_TRUE(wc.isInCategory(Application));
                EXPECT_FALSE(wc.isInCategory(Mouse));
            }

            TEST(EventTest, WindowResizeSimple) {
                const uint32_t width = 220;
                const uint32_t height = 144;
                const double widthFactor = 1.0;
                const double heightFactor = 0.5;
                WindowResize wr(width, height, widthFactor, heightFactor);

                EXPECT_EQ(wr.getType(), Type::WindowResize);
                EXPECT_TRUE(wr.isInCategory(Application));
                EXPECT_FALSE(wr.isInCategory(Mouse));

                EXPECT_EQ(wr.getWidth(), width);
                EXPECT_EQ(wr.getHeight(), height);
                EXPECT_EQ(wr.getWidthFactor(), widthFactor);
                EXPECT_EQ(wr.getHeightFactor(), heightFactor);
                EXPECT_EQ(wr.getFrameBufferWidth(), width);
                EXPECT_EQ(wr.getFrameBufferHeight(), height);
                // custom to string
                EXPECT_EQ(wr.toString(), "WindowResize(220,144,1,0.5,220,144)");
            }

            TEST(EventTest, WindowResizeDiffFrameBufferSize) {
                const uint32_t width = 220;
                const uint32_t height = 144;
                const double widthFactor = 1.0;
                const double heightFactor = 0.5;
                const uint32_t fbWidth = 440;
                const uint32_t fbHeight = 288;
                WindowResize wr(width, height, widthFactor, heightFactor, fbWidth, fbHeight);

                EXPECT_EQ(wr.getType(), Type::WindowResize);
                EXPECT_TRUE(wr.isInCategory(Application));
                EXPECT_FALSE(wr.isInCategory(Mouse));

                EXPECT_EQ(wr.getWidth(), width);
                EXPECT_EQ(wr.getHeight(), height);
                EXPECT_EQ(wr.getWidthFactor(), widthFactor);
                EXPECT_EQ(wr.getHeightFactor(), heightFactor);
                EXPECT_EQ(wr.getFrameBufferWidth(), fbWidth);
                EXPECT_EQ(wr.getFrameBufferHeight(), fbHeight);
                // custom to string
                EXPECT_EQ(wr.toString(), "WindowResize(220,144,1,0.5,440,288)");
            }

        }
    }
}
