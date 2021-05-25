/** @file */
#ifndef _CORE_EVENTS_MOUSE_H
#define _CORE_EVENTS_MOUSE_H

#include <sstream>
#include "core/events/event.hpp"
#include "core/events/key.hpp"

namespace tme {
    namespace core {
        namespace events {
            
            /**//**
             * \brief Event emitted when the mouse is moved.
             *
             * Contains the new mouse coordinates in absolute and relative form 
             * accessible via getters.
             * Coordinates are measured from top left to bottom right.
             * Absolute coordinates are in pixels.
             * Relative coordinates are between 0.0 and 1.0.
             */
            class MouseMove : public Event {
                double m_xPos, m_yPos, m_xPosRel, m_yPosRel;

                public:
                /**//**
                 * \brief Construct MouseMove event form absolute and relative mouse position.
                 *
                 * @param xPos absolute x position
                 * @param yPos absolute y position
                 * @param xPosRel relative x position
                 * @param yPosRel relative y position
                 */
                MouseMove(double xPos, double yPos, double xPosRel, double yPosRel) : m_xPos(xPos), m_yPos(yPos), m_xPosRel(xPosRel), m_yPosRel(yPosRel) {}

                EVENT_CLASS_TYPE(MouseMove)
                EVENT_CLASS_CATEGORY(Input | Mouse)

                /**//**
                 * \brief Get absolute x pos.
                 *
                 * @return absolute mouse x position
                 */
                double getXPos() const { return m_xPos; }
                /**//**
                 * \brief Get absolute y pos.
                 *
                 * @return absolute mouse y position
                 */
                double getYPos() const { return m_yPos; }
                /**//**
                 * \brief Get relative x pos.
                 *
                 * @return relative mouse x position
                 */
                double getXPosRel() const { return m_xPosRel; }
                /**//**
                 * \brief Get relative y pos.
                 *
                 * @return relative mouse y position
                 */
                double getYPosRel() const { return m_yPosRel; }

                std::string toString() const override {
                    std::stringstream ss;
                    ss << getName() << '(';
                    ss << m_xPos << ',';
                    ss << m_yPos << ',';
                    ss << m_xPosRel << ',';
                    ss << m_yPosRel << ')';
                    return ss.str();
                }
            };

            /**//**
             * \brief Event emitted when a mouse wheel is scrolled.
             *
             * Contains the scrolled offset in x and y direction.
             */
            class MouseScroll : public Event {
                double m_xOffset, m_yOffset;

                public:
                /**//**
                 * \brief Construct MouseScroll instance from scroll offsets.
                 *
                 * @param xOffset distance scrolled in x direction
                 * @param yOffset distance scrolled in y direction
                 */
                MouseScroll(double xOffset, double yOffset) : m_xOffset(xOffset), m_yOffset(yOffset) {}

                /**//**
                 * \brief Get x scroll offset.
                 *
                 * @return scroll offset in x direction
                 */
                double getXOffset() const { return m_xOffset; }
                /**//**
                 * \brief Get y scroll offset.
                 *
                 * @return scroll offset in y direction
                 */
                double getYOffset() const { return m_yOffset; }

                EVENT_CLASS_TYPE(MouseScroll)
                EVENT_CLASS_CATEGORY(Input | Mouse)

                std::string toString() const override {
                    std::stringstream ss;
                    ss << getName() << '(' << m_xOffset << ',' << m_yOffset << ')';
                    return ss.str();
                }
            };

            /**//**
             * \brief Event emitted when a mouse button is pressed.
             *
             * Basically a typesafe wrapper for KeyPress changing the
             * associated categories.
             */
            class MouseKeyPress : public KeyPress {
                public:
                /**//**
                 * \brief Construct MouseKeyPress instance from key.
                 *
                 * It forewards the key to KeyPress.
                 *
                 * @param key the key that was pressed
                 */
                MouseKeyPress(const Key& key) : KeyPress(key) {}

                EVENT_CLASS_TYPE(MouseKeyPress)
                EVENT_CLASS_CATEGORY(Input | Mouse)
            };

            /**//**
             * \brief Event emitted when a mouse button is released.
             *
             * Basically a typesafe wrapper for KeyRelease changing the
             * associated categories.
             */
            class MouseKeyRelease : public KeyRelease {
                public:
                /**//**
                 * \brief Construct MouseKeyRelease instance from key.
                 *
                 * It forewards the key to KeyPress.
                 *
                 * @param key the key that was released
                 */
                MouseKeyRelease(const Key& key) : KeyRelease(key) {}

                EVENT_CLASS_TYPE(MouseKeyRelease)
                EVENT_CLASS_CATEGORY(Input | Mouse)
            };

        }
    }
}

#endif

