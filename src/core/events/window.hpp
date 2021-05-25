#ifndef _CORE_EVENTS_WINDOW_H
#define _CORE_EVENTS_WINDOW_H
/** @file */

#include "core/events/event.hpp"
#include <sstream>

namespace tme {
    namespace core {
        namespace events {

            /**//**
             * \brief Event emitted when the window updates.
             *
             * Contains the time since the last update in seconds.
             */
            class WindowUpdate : public Event {
                double m_deltaTime;
                public:
                /**//**
                 * \brief Construct WindowUpdate event with new time delta.
                 *
                 * @param deltaTime amount of time passed since the last update
                 */
                WindowUpdate(double deltaTime) : m_deltaTime(deltaTime) {}

                /**//**
                 * \brief Get time delta since last update.
                 *
                 * @return time delta since last update in seconds
                 */
                inline double getDeltaTime() const { return m_deltaTime; }

                EVENT_CLASS_TYPE(WindowUpdate)
                EVENT_CLASS_CATEGORY(Application)

                std::string toString() const override {
                    std::stringstream ss;
                    ss << getName() << '(' << m_deltaTime << ')';
                    return ss.str();
                }
            };

            /**//**
             * \brief Event when Window is closed.
             */
            class WindowClose : public Event {
                public:
                /**//**
                 * \brief Construct WindowClose event.
                 */
                WindowClose() {}

                EVENT_CLASS_TYPE(WindowClose)
                EVENT_CLASS_CATEGORY(Application)
            };

            /**//**
             * \brief Event when Window is resized.
             *
             * Contains new window dimensions, the scaling factors compared to the previous
             * dimensions and new frame buffer dimensions.
             * The two dimension types are stored because they can differ on high dpi displays.
             */
            class WindowResize : public Event {
                uint32_t m_width, m_height;
                double m_widthFactor, m_heightFactor;
                uint32_t m_frameBufferWidth, m_frameBufferHeight;

                public:
                /**//**
                 * \brief Construct WindowResize event.
                 *
                 * @param width new width of the window
                 * @param height new height of the window
                 * @param widthFactor width scaling factor compared to previous width
                 * @param heightFactor height scaling factor compared to previous height
                 * @param frameBufferWidth new width of frame buffer
                 * @param frameBufferHeight new height of the frame buffer
                 */
                WindowResize(uint32_t width, uint32_t height, double widthFactor, double heightFactor, uint32_t frameBufferWidth, uint32_t frameBufferHeight)
                    : m_width(width), m_height(height),
                    m_widthFactor(widthFactor), m_heightFactor(heightFactor),
                    m_frameBufferWidth(frameBufferWidth), m_frameBufferHeight(frameBufferHeight) {}
                /**//**
                 * \brief Construct WindowResize event with new window dimensions.
                 *
                 * Uses new window dimensions as new frame buffer dimensions.
                 *
                 * @param width new width of the window
                 * @param height new height of the window
                 * @param widthFactor width scaling factor compared to previous width
                 * @param heightFactor height scaling factor compared to previous height
                 */
                WindowResize(uint32_t width, uint32_t height, double widthFactor, double heightFactor) 
                    : WindowResize(width, height, widthFactor, heightFactor, width, height) {}

                /**//**
                 * \brief Get new window width.
                 *
                 * @return new window width
                 */
                inline auto getWidth() const { return m_width; }

                /**//**
                 * \brief Get new window height.
                 *
                 * @return new window height
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

                /**//**
                 * \brief Get new frame buffer width.
                 *
                 * @return new frame buffer width
                 */
                inline auto getFrameBufferWidth() const { return m_frameBufferWidth; }

                /**//**
                 * \brief Get new frame buffer height.
                 *
                 * @return new frame buffer height
                 */
                inline auto getFrameBufferHeight() const { return m_frameBufferHeight; }

                std::string toString() const override {
                    std::stringstream ss;
                    ss << getName() << '(' << m_width << ',' << m_height << ',';
                    ss << m_widthFactor << ',' << m_heightFactor << ',';
                    ss << m_frameBufferWidth << ',' << m_frameBufferHeight << ')';
                    return ss.str();
                }

                EVENT_CLASS_TYPE(WindowResize)
                EVENT_CLASS_CATEGORY(Application)
            };

        }
    }
}

#endif
