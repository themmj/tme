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

            TEST(EventTest, KeyPress) {
                _DefaultKey dk(TME_KEY_H);
                KeyPress kp(dk);

                EXPECT_EQ(dk.getKeyCode(),kp.getKey().getKeyCode());
                EXPECT_EQ(kp.getType(), Type::KeyPress);
                EXPECT_TRUE(kp.isInCategory(Category::Input));
                EXPECT_TRUE(kp.isInCategory(Category::Keyboard));
                EXPECT_EQ(kp.toString(), "KeyPress(25,000000)");
            }

            TEST(EventTest, KeyRelease) {
                _DefaultKey dk(TME_KEY_K);
                KeyRelease kr(dk);

                EXPECT_EQ(dk.getKeyCode(),kr.getKey().getKeyCode());
                EXPECT_EQ(kr.getType(), Type::KeyRelease);
                EXPECT_TRUE(kr.isInCategory(Category::Input));
                EXPECT_TRUE(kr.isInCategory(Category::Keyboard));
                EXPECT_EQ(kr.toString(), "KeyRelease(28,000000)");
            }

            TEST(EventTest, KeyChar) {
                _DefaultKey dk(0x0000004e);
                KeyChar kr(dk);

                EXPECT_EQ(dk.getKeyCode(),kr.getKey().getKeyCode());
                EXPECT_EQ(kr.getType(), Type::KeyChar);
                EXPECT_TRUE(kr.isInCategory(Category::Input));
                EXPECT_TRUE(kr.isInCategory(Category::Keyboard));
                EXPECT_EQ(kr.toString(), "KeyChar(78)");
            }

        }
    }
}
