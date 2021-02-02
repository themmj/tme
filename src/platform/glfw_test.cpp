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

    }
}

