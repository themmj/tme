#include "gtest/gtest.h"
#include "core/events/event.hpp"
#include "core/events/window.hpp"
#include "core/layers/layer.hpp"
#include "core/events/dispatcher.hpp"

namespace tme {
    namespace core {
        namespace layers {

            class _NamedLayer final : public Layer {
                public:
                _NamedLayer(const std::string& name) : Layer(name) {}

                void render() override {}
            };

            class _CounterLayer final : public Layer, events::Dispatcher<_CounterLayer> {
                public:
                uint32_t m_counter;

                _CounterLayer(uint32_t start) : Layer("Default"), Dispatcher(this), m_counter(start) {}

                void onEvent(events::Event& e) override {
                    dispatchEvent<events::WindowClose>(e, &_CounterLayer::handleWindowClose);
                }

                bool handleWindowClose(events::WindowClose&) {
                    ++m_counter;
                    return true;
                }

                void render() override { --m_counter; }

                std::string toString() const override {
                    std::stringstream ss;
                    ss << m_counter;
                    return ss.str();
                }
            };

            TEST(TestLayer, DefaultToString) {
                auto testString = "TestString";
                _NamedLayer l(testString);
                EXPECT_EQ(l.toString(), testString);
            }

            TEST(TestLayer, DefaultOnEvent) {
                // onEvent is empty by default so it should
                // not change the state
                auto testString = "TestString";
                events::WindowClose wc;
                _NamedLayer l(testString);
                _NamedLayer l1 = l;
                l.onEvent(wc);
                EXPECT_EQ(l.toString(), l1.toString());
            }

            TEST(TestLayerStack, Push) {
                Stack s;
                s.push<_CounterLayer>(1);
                s.push<_CounterLayer>(4);

                EXPECT_EQ(s.toString(), "LayerStack( 4 1 )");
            }

            TEST(TestLayerStack, Pop) {
                Stack s;
                bool res;
                s.push<_CounterLayer>(1);
                s.push<_CounterLayer>(4);
                res = s.pop();
                EXPECT_TRUE(res);
                EXPECT_EQ(s.toString(), "LayerStack( 1 )");
                res = s.pop();
                EXPECT_TRUE(res);
                // should be empty now
                res = s.pop();
                EXPECT_FALSE(res);
            }

            TEST(TestLayerStack, OnEvent) {
                Stack s;
                s.push<_CounterLayer>(1);
                s.push<_CounterLayer>(4);
                
                events::WindowClose wc;
                s.onEvent(wc);

                EXPECT_EQ(s.toString(), "LayerStack( 5 1 )");
            }

            TEST(TestLayerStack, Render) {
                Stack s;
                s.push<_CounterLayer>(1);
                s.push<_CounterLayer>(4);
                
                s.render();

                EXPECT_EQ(s.toString(), "LayerStack( 3 0 )");
            }

        }
    }
}
