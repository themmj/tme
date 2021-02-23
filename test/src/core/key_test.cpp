#include "gtest/gtest.h"
#include "core/key.hpp"

namespace tme {
    namespace core {

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
    
        TEST(TestKey, Construction) {
            _DefaultKey dk(TME_KEY_E);
            EXPECT_EQ(dk.getKeyCode(), TME_KEY_E);
        }

        TEST(TestKey, ToString) {
            _DefaultKey dk(TME_KEY_M);
            EXPECT_EQ(dk.toString(), "Key(30)");
        }

    }
}
