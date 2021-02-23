#include "gtest/gtest.h"
#include "core/events/event.hpp"

namespace tme {
    namespace core {
        namespace events {

            class _DefaultEvent : public Event {
                public:
                _DefaultEvent() {}

                void setHandled(bool handled) { m_handled = handled; }

                EVENT_CLASS_TYPE(WindowClose)
                EVENT_CLASS_CATEGORY(Application | Input)
            };

            TEST(EventTest, DefaultToString) {
                _DefaultEvent de;
                EXPECT_EQ(de.toString(), de.getName());
            }

            TEST(EventTest, IsHandled) {
                _DefaultEvent de;
                EXPECT_FALSE(de.isHandled());
                de.setHandled(true);
                EXPECT_TRUE(de.isHandled());
            }

            TEST(EventTest, TypeMacros) {
                _DefaultEvent de;
                EXPECT_EQ(de.getType(), _DefaultEvent::getStaticType());
            }

            TEST(EventTest, CategoryMacros) {
                _DefaultEvent de;
                EXPECT_TRUE(de.isInCategory(Category::Application) && de.isInCategory(Category::Input));
            }

        }
    }
}

