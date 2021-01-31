#include "gtest/gtest.h"
#include "core/events/event.hpp"
#include "core/events/window.hpp"

namespace tme {
    namespace core {
        namespace events {

            TEST(EventTest, TypeMacros) {
                WindowClose closeEvent;
                EXPECT_EQ(closeEvent.getType(), WindowClose::getStaticType());
                EXPECT_EQ(std::string("WindowClose").compare(closeEvent.getName()), 0);
            }

            TEST(EventTest, CategoryMacros) {
                WindowClose closeEvent;
                EXPECT_TRUE(closeEvent.isInCategory(Category::Application));
            }

        }
    }
}

