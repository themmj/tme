#ifndef _TEST_GRAPHICS_BASE_H
#define _TEST_GRAPHICS_BASE_H

#include "gtest/gtest.h"
#include <memory>

#include "platform/context.hpp"
#include "core/window.hpp"

namespace tme {
    namespace core {
        namespace graphics {

            class GraphicsTest : public ::testing::Test {
                protected:
                std::unique_ptr<platform::Context> m_context;
                Identifier m_window;

                virtual void SetUp() override {
                    m_context = platform::Context::create();
                    m_window = Storage<Window>::global()->add(Window::create({nullptr, "This is a test", 640, 300, true}))->getId();
                }

                virtual void TearDown() override {
                    Storage<Window>::global()->destroy(m_window);
                }
            };

        }
    }
}

#endif

