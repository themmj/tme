#include "gtest//gtest.h"
#include "core/events/dispatcher.hpp"
#include "core/events/window.hpp"

namespace tme {
    namespace core {
        namespace events {

            class _TestDispatcher : public Dispatcher<_TestDispatcher> {
                uint32_t m_hitMask;
                
                template<typename EventType>
                bool handleEvent(EventType&) {
                    m_hitMask |= BIT(static_cast<uint32_t>(EventType::getStaticType()));
                    return true;
                }

                public:
                _TestDispatcher() : m_hitMask(0) {}

                template<typename EventType>
                bool hitEvent() const {
                    return m_hitMask & BIT(static_cast<uint32_t>(EventType::getStaticType()));
                }

                void onEvent(Event& event) override {
                    dispatchEvent<WindowClose>(event, this, &_TestDispatcher::handleEvent<WindowClose>);
                }
            };

            
            TEST(DispatcherTest, DispatchSingleEvent) {
                _TestDispatcher td;
                EXPECT_FALSE(td.hitEvent<WindowClose>());
                WindowClose event;
                td.onEvent(event);
                EXPECT_TRUE(td.hitEvent<WindowClose>());
            }

            TEST(DispatcherTest, DispatchMismatchedEvent) {
                _TestDispatcher td;
                EXPECT_FALSE(td.hitEvent<WindowResize>());
                WindowResize event(640, 300);
                td.onEvent(event);
                EXPECT_FALSE(td.hitEvent<WindowResize>());
                EXPECT_FALSE(td.hitEvent<WindowClose>());
            }

        }
    }
}

