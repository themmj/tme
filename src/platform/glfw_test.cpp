#include "gtest/gtest.h"
#include "core/window.hpp"
#include "platform/glfw.hpp"
#include "core/events/handler.hpp"

namespace tme {
    namespace platform {

        class _BlankHandler : public core::events::Handler {
            public:
            void onEvent(core::events::Event&) override {}
        };

        TEST(TestGlfwWindow, CreationAndDestruction) {
            _BlankHandler bh;
            EXPECT_FALSE(GlfwWindow::s_glfwInitialized);
            EXPECT_EQ(core::Window::s_windowCount, 0);
            // tests static create method implementation in header
            auto gw = core::Window::create({"test title", 220, 144, &bh});
            EXPECT_TRUE(GlfwWindow::s_glfwInitialized);
            EXPECT_EQ(core::Window::s_windowCount, 1);
            // execute overriden functions to check for errors, can't really
            // validate more than that right now
            gw->onUpdate();
            gw->setTitle("other title");
            gw->setVSync(!gw->isVSync());
            // cleanup as this pointer owns;
            delete gw;
            EXPECT_EQ(core::Window::s_windowCount, 0);
        }

        TEST(TestGlfwWindow, HandleUninitializedGlfwCreation) {
            _BlankHandler bh;
            // set to true to pretend glfw has been initialized
            // when it has in fact not been initialized yet
            GlfwWindow::s_glfwInitialized = true;
            EXPECT_EQ(core::Window::s_windowCount, 0);
            // should not increment the window counter as it does not
            // create one
            core::Window::create({"test title", 220, 144, &bh});
            EXPECT_EQ(core::Window::s_windowCount, 0);
            // reset state
            GlfwWindow::s_glfwInitialized = false;
        }

        TEST(TestGlfwKey, KeyMapping) {
            GlfwKey key(GLFW_KEY_F, 0);
            EXPECT_EQ(key.getKeyCode(), TME_KEY_F);
            EXPECT_TRUE(key.isKey(TME_KEY_F));
        }

        TEST(TestGlfwKey, KeyMods) {
            GlfwKey key(GLFW_KEY_L, GLFW_MOD_SHIFT | GLFW_MOD_ALT);
            EXPECT_TRUE(key.hasModAlt());
            EXPECT_TRUE(key.hasModShift());
            EXPECT_FALSE(key.hasModSuper());
            EXPECT_FALSE(key.hasModControl());
            EXPECT_FALSE(key.hasModNumLock());
            EXPECT_FALSE(key.hasModCapsLock());
        }

        TEST(TestGlfwKey, UnknownKey) {
            // this key is not mapped to it should
            // result in an TME_KEY_UNKNOW
            GlfwKey key(GLFW_KEY_PRINT_SCREEN, 0);
            EXPECT_EQ(key.getKeyCode(), TME_KEY_UNKNOWN);
        }

    }
}

