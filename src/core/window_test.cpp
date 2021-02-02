#include "gtest/gtest.h"
#include "core/window.hpp"
#include "core/events/dispatcher.hpp"
#include "core/events/window.hpp"

namespace tme {
    namespace core {

        class _DefaultWindow : public Window {
            public:
            _DefaultWindow(const Window::Data& data) : Window(data) { ++s_windowCount; }
            ~_DefaultWindow() { --s_windowCount; }
            using Window::Dimension;
            void onUpdate() override {}
            void setTitle(const std::string& title) override { m_data.title = title; }
            void setVSync(bool enable) override { m_data.vSyncEnabled = enable; }
            void sendEvent() { events::WindowClose wc; m_data.handler->onEvent(wc); }
        };

        class _DefaultWindowEventHandler : public events::Dispatcher<_DefaultWindowEventHandler> {
            public:
            bool processedWindowClose = false;

            _DefaultWindowEventHandler() : Dispatcher(this) {}

            void onEvent(events::Event& event) override {
                dispatchEvent<events::WindowClose>(event, &_DefaultWindowEventHandler::handleWindowClose);
            }
            bool handleWindowClose(events::WindowClose&) {
                processedWindowClose = true;
                return true;
            }
        };

        TEST(WindowTest, DefaultWindow) {
            std::string title{"Test title"};
            _DefaultWindow::Dimension width = 240;
            _DefaultWindow::Dimension height = 144;
            _DefaultWindowEventHandler deh;

            EXPECT_EQ(Window::s_windowCount, 0);
            _DefaultWindow dw(Window::Data{title, width, height, &deh});
            EXPECT_EQ(Window::s_windowCount, 1);

            EXPECT_EQ(dw.getTitle(), title);
            EXPECT_EQ(dw.getWidth(), width);
            EXPECT_EQ(dw.getHeight(), height);
            EXPECT_FALSE(dw.isVSync());

            EXPECT_EQ(dw.toString(), "Window: Test title(240,144,0)");
        }

    }
}
