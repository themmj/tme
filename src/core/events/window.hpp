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

                EVENT_CLASS_TYPE(WindowClose)
                EVENT_CLASS_CATEGORY(Application)
            };

            /**//**
             * \brief Event when window is resized.
             */
            class WindowResize : public Event {
                uint32_t m_width, m_height;
                double m_widthFactor, m_heightFactor;

                public:
                /// Construct WindowResize event with new window dimensions.
                WindowResize(uint32_t width, uint32_t height, double widthFactor, double heightFactor) 
                    : m_width(width), m_height(height), m_widthFactor(widthFactor), m_heightFactor(heightFactor) {}

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

                /**//**
                 * \brief Get factor of new width.
                 * 
                 * i.e.
                 * oldWidth = 1000
                 * newWidth = 500
                 * -> factor 0.5
                 *
                 * @return factor of the new width in relation to the old one
                 */
                inline auto getWidthFactor() const { return m_widthFactor; }

                /**//**
                 * \brief Get factor of new height.
                 *
                 * i.e.
                 * oldWidth = 1000
                 * newWidth = 500
                 * -> factor 0.5
                 *
                 * @return factor of the new height in relation to the old one
                 */
                inline auto getHeightFactor() const { return m_heightFactor; }

                std::string toString() const override {
                    std::stringstream ss;
                    ss << getName() << '(' << m_width << ',' << m_height << ',';
                    ss << m_widthFactor << ',' << m_heightFactor << ')';
                    return ss.str();
                }

                EVENT_CLASS_TYPE(WindowResize)
                EVENT_CLASS_CATEGORY(Application)
            };

        }
    }
}

#endif
