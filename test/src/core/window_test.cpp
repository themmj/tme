#include "gtest/gtest.h"
#include "core/window.hpp"
#include "core/events/dispatcher.hpp"
#include "core/events/window.hpp"

namespace tme {
    namespace core {

        class _DefaultWindow : public Window {
            public:
            _DefaultWindow(const Window::Data& data) : Window(data) {}
            ~_DefaultWindow() {}
            using Window::Dimension;
            void update() override {}
            void setTitleInternal(const std::string& title) override { m_data.title = title; }
            void setVSyncInternal(bool enable) override { m_data.vSyncEnabled = enable; }
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

            _DefaultWindow dw(Window::Data{&deh, title, width, height});

            EXPECT_EQ(dw.getTitle(), title);
            EXPECT_EQ(dw.getWidth(), width);
            EXPECT_EQ(dw.getHeight(), height);
            EXPECT_TRUE(dw.isVSync());

            EXPECT_EQ(dw.toString(), "Window: 0(Test title,240,144,1)");
        }

        TEST(WindowTest, RelativeCoordinates) {
            std::string title{"Test title"};
            _DefaultWindow::Dimension width = 240;
            _DefaultWindow::Dimension height = 144;
            _DefaultWindowEventHandler deh;
            _DefaultWindow dw(Window::Data{&deh, title, width, height});

            EXPECT_EQ(dw.getRelativeX(120.0), 0.5);
            EXPECT_EQ(dw.getRelativeY(72.0), 0.5);

            EXPECT_EQ(dw.getRelativeX(60), 0.25);
            EXPECT_EQ(dw.getRelativeY(36), 0.25);
        }

    }
}
