#include "gtest/gtest.h"
#include "core/events/key.hpp"

namespace tme {
    namespace core {
        namespace events {

            class _DefaultKey : public core::Key {
                public:
                _DefaultKey(int32_t keyCode) : Key(keyCode) {}

                bool hasModShift() const override {
                    return false;
                }
                bool hasModControl() const override {
                    return false;
                }
                bool hasModAlt() const override {
                    return false;
                }
                bool hasModSuper() const override {
                    return false;
                }
                bool hasModCapsLock() const override {
                    return false;
                }
                bool hasModNumLock() const override {
                    return false;
                }
            };

            TEST(EventTest, KeyPressed) {
                _DefaultKey dk(TME_KEY_H);
                KeyPressed kp(dk);

                EXPECT_EQ(dk.getKeyCode(),kp.getKey().getKeyCode());
                EXPECT_EQ(kp.getType(), Type::KeyPressed);
                EXPECT_TRUE(kp.isInCategory(Category::Input));
                EXPECT_TRUE(kp.isInCategory(Category::Keyboard));
                EXPECT_EQ(kp.toString(), "KeyPressed(25,000000)");
            }

            TEST(EventTest, KeyReleased) {
                _DefaultKey dk(TME_KEY_K);
                KeyReleased kr(dk);

                EXPECT_EQ(dk.getKeyCode(),kr.getKey().getKeyCode());
                EXPECT_EQ(kr.getType(), Type::KeyReleased);
                EXPECT_TRUE(kr.isInCategory(Category::Input));
                EXPECT_TRUE(kr.isInCategory(Category::Keyboard));
                EXPECT_EQ(kr.toString(), "KeyReleased(28,000000)");
            }

            TEST(EventTest, KeyTyped) {
                _DefaultKey dk(0x0000004e);
                KeyTyped kr(dk);

                EXPECT_EQ(dk.getKeyCode(),kr.getKey().getKeyCode());
                EXPECT_EQ(kr.getType(), Type::KeyTyped);
                EXPECT_TRUE(kr.isInCategory(Category::Input));
                EXPECT_TRUE(kr.isInCategory(Category::Keyboard));
                EXPECT_EQ(kr.toString(), "KeyTyped(78)");
            }

        }
    }
}
