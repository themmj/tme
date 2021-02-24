#include "gtest/gtest.h"
#include "core/events/event.hpp"
#include "core/events/window.hpp"
#include "core/layers/layer.hpp"
#include "core/events/dispatcher.hpp"

namespace tme {
    namespace core {
        namespace layers {

            class _DefaultLayer : public Layer, events::Dispatcher<_DefaultLayer> {
                public:
                uint32_t m_counter;

                _DefaultLayer(uint32_t start) : Layer("Default"), Dispatcher(this), m_counter(start) {}

                void onEvent(events::Event& e) override {
                    dispatchEvent<events::WindowClose>(e, &_DefaultLayer::handleWindowClose);
                }

                bool handleWindowClose(events::WindowClose&) {
                    ++m_counter;
                    return true;
                }

                std::string toString() const override {
                    std::stringstream ss;
                    ss << m_counter;
                    return ss.str();
                }
            };

            TEST(TestLayer, DefaultToString) {
                auto testString = "TestString";
                Layer l(testString);
                EXPECT_EQ(l.toString(), testString);
            }

            TEST(TestLayer, DefaultOnEvent) {
                // onEvent is empty by default so it should
                // not change the state
                auto testString = "TestString";
                events::WindowClose wc;
                Layer l(testString);
                Layer l1 = l;
                l.onEvent(wc);
                EXPECT_EQ(l.toString(), l1.toString());
            }

            TEST(TestLayer, LayerStack) {
                // the WindowClose event will only be
                // executed by the top layer
                // this check if its the last added layer
                // and if the remaining layer is unchanged
                Stack s;
                s.push<_DefaultLayer>(1);
                s.push<_DefaultLayer>(4);

                events::WindowClose wc;
                s.onEvent(wc);

                EXPECT_EQ(s.toString(), "LayerStack( 5 1 )");
            }

        }
    }
}
