#include "gtest/gtest.h"
#include <memory>
#include <string>
#include "sigcounter.hpp"
#include "core/window.hpp"
#include "platform/glfw.hpp"
#include "core/events/handler.hpp"

namespace tme {
    namespace platform {

        class _BlankHandler : public core::events::Handler {
            public:
            void onEvent(core::events::Event&) override {}
        };

        TEST(GlfwWindowTest, CreationAndDestruction) {
            int assertCountBefore = SignalCounter::instance()->get(SignalCounter::assertionFailed);
            auto context = Context::create();
            _BlankHandler bh;
            // tests static create method implementation in header
            auto gw = core::Window::create({&bh, "test title", 220, 144});
            // execute overriden functions to check for errors, can't really
            // validate more than that right now
            gw->update();
            gw->setTitle("other title");
            gw->setVSync(!gw->isVSync());
            // cleanup as this pointer owns;
            delete gw;
            // should not cause any assert errors
            EXPECT_EQ(SignalCounter::instance()->get(SignalCounter::assertionFailed), assertCountBefore);
        }

        TEST(GlfwWindowTest, CreationWithoutContext) {
            // should invoke a glfw error to cover that code path
            // triggers an assert failure which is checked
            _BlankHandler bh;
            int assertCountBefore = SignalCounter::instance()->get(SignalCounter::assertionFailed);
            auto gw = core::Window::create({&bh, "test title", 220, 144});
            EXPECT_EQ(SignalCounter::instance()->get(SignalCounter::assertionFailed), assertCountBefore + 1);
            delete gw;
        }

        TEST(GlfwKeyTest, KeyMapping) {
            GlfwKey key(GLFW_KEY_F, 0);
            EXPECT_EQ(key.getKeyCode(), TME_KEY_F);
            EXPECT_TRUE(key.isKey(TME_KEY_F));
        }

        TEST(GlfwKeyTest, KeyMods) {
            GlfwKey key(GLFW_KEY_L, GLFW_MOD_SHIFT | GLFW_MOD_ALT);
            EXPECT_TRUE(key.hasModAlt());
            EXPECT_TRUE(key.hasModShift());
            EXPECT_FALSE(key.hasModSuper());
            EXPECT_FALSE(key.hasModControl());
            EXPECT_FALSE(key.hasModNumLock());
            EXPECT_FALSE(key.hasModCapsLock());
        }

        TEST(GlfwKeyTest, UnknownKey) {
            // this key is not mapped to it should
            // result in an TME_KEY_UNKNOW
            GlfwKey key(GLFW_KEY_PRINT_SCREEN, 0);
            EXPECT_EQ(key.getKeyCode(), TME_KEY_UNKNOWN);
        }

        TEST(GlfwKeyTest, CharValue) {
            GlfwCharKey key(0x0000004c); // capital L
            EXPECT_EQ(static_cast<char32_t>(key.getKeyCode()), U'L');
        }

        TEST(GlfwKeyTest, ModStubs) {
            GlfwCharKey key(0x0000004d);
            EXPECT_FALSE(key.hasModAlt());
            EXPECT_FALSE(key.hasModShift());
            EXPECT_FALSE(key.hasModSuper());
            EXPECT_FALSE(key.hasModControl());
            EXPECT_FALSE(key.hasModNumLock());
            EXPECT_FALSE(key.hasModCapsLock());
        }

    }
}

