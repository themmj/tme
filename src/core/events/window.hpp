#ifndef _CORE_EVENTS_WINDOW_H
#define _CORE_EVENTS_WINDOW_H
/** @file */

#include "core/events/event.hpp"
#include <sstream>

namespace tme {
    namespace core {
        namespace events {

            /**//**
             * \brief Event when window is closed.
             */
            class WindowClose : public Event {
                public:
                WindowClose() {}

                /// @cond INCLUDE_MACROS GCOVR_EXCL_START
                EVENT_CLASS_TYPE(WindowClose);
                EVENT_CLASS_CATEGORY(Application);
                /// @endcond GCOVR_EXCL_STOP
            };

            /**//**
             * \brief Event when window is resized.
             */
            class WindowResize : public Event {
                uint32_t m_width, m_height;

                public:
                /// Construct WindowResize event with new window dimensions.
                WindowResize(uint32_t width, uint32_t height) : m_width(width), m_height(height) {}

                /**//**
                 * \brief Get new window width.
                 *
                 * @return new window width.
                 */
                inline auto getWidth() const { return m_width; }

                /**//**
                 * \brief Get new window height.
                 *
                 * @return new window height.
                 */
                inline auto getHeight() const { return m_height; }

                /// @cond INCLUDE_MACROS GCOVR_EXCL_START
                std::string toString() const override {
                    std::stringstream ss;
                    ss << getName() << '(' << m_width << ',' << m_height << ')';
                    return ss.str();
                }

                EVENT_CLASS_TYPE(WindowResize);
                EVENT_CLASS_CATEGORY(Application);
                /// @endcond GCOVR_EXCL_STOP
            };

        }
    }
}

#endif
