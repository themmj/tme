#include "gtest/gtest.h"
#include "core/application.hpp"
#include "core/events/window.hpp"
#include "platform/context.hpp"
#include "sigcounter.hpp"

namespace tme {
    namespace core {

        // the base Application class only provides a way to identify it
        // and a virtual run method. it will be tested indirectly by the
        // WindowApplication tests

        class _TestWindowApp : public WindowApplication {
            int m_iterations = 0;
            public:
            _TestWindowApp() : WindowApplication("_TestApp") {}
            ~_TestWindowApp() {}

            void update() override {
                if (m_iterations > 1) {
                    events::WindowClose wc;
                    onEvent(wc);
                }
                ++m_iterations;
            }
        };

        TEST(TestApplication, WindowApplicationCreation) {
            int assertCountBefore = SignalCounter::instance()->get(SignalCounter::assertionFailed);
            auto context = platform::Context::create();
            auto ta = Storage<_TestWindowApp>::global()->create();
            ta->run();
            Storage<_TestWindowApp>::global()->clear();
            EXPECT_EQ(SignalCounter::instance()->get(SignalCounter::assertionFailed), assertCountBefore);
        }

    }
}

