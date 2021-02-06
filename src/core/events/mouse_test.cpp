#include "gtest/gtest.h"
#include "core/events/mouse.hpp"

namespace tme {
    namespace core {
        namespace events {

            TEST(EventTest, MouseScroll) {
                MouseScroll ms(1.0, 0);
                EXPECT_EQ(ms.getType(), Type::MouseScroll);
                EXPECT_TRUE(ms.isInCategory(Category::Input));
                EXPECT_TRUE(ms.isInCategory(Category::Mouse));
                EXPECT_EQ(ms.toString(), "MouseScroll(1,0)");
            }

            TEST(EventTest, MouseMove) {
                MouseMove mm(100, 200, 0.5, 0.75);
                EXPECT_EQ(mm.getType(), Type::MouseMove);
                EXPECT_TRUE(mm.isInCategory(Category::Input));
                EXPECT_TRUE(mm.isInCategory(Category::Mouse));
                EXPECT_EQ(mm.toString(), "MouseMove(100,200,0.5,0.75)");
            }

            // KeyPress / -Release specialisations are tested inside core/events/key_test.cpp

        }
    }
}
