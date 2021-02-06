/** @file */
#ifndef _CORE_EVENTS_KEY_H
#define _CORE_EVENTS_KEY_H

#include <sstream>
#include "core/events/event.hpp"
#include "core/key.hpp"

namespace tme {
    namespace core {
        namespace events {

            /**//**
             * \brief Base class for a key event.
             * Contains the key itself, getters and the
             * necessary category and toString overloads.
             */
            class KeyBase : public Event {
                protected:
                /// The key for which the event was triggered
                const Key& m_key;
                
                public:
                /// Construct a KeyBase instance containing the key.
                KeyBase(const Key& key) : m_key(key) {}
                virtual ~KeyBase() {}

                /**//**
                 * \brief Get the key.
                 *
                 * @return The key for which the event was triggered.
                 */
                const Key& getKey() const { return m_key; }

                EVENT_CLASS_CATEGORY(Input | Keyboard)
                
                virtual std::string toString() const override {
                    std::stringstream ss;
                    ss << getName() << '(' << m_key.getKeyCode() << ',';
                    ss << m_key.hasModShift();
                    ss << m_key.hasModControl();
                    ss << m_key.hasModAlt();
                    ss << m_key.hasModSuper();
                    ss << m_key.hasModCapsLock();
                    ss << m_key.hasModNumLock(); 
                    ss << ')';
                    return ss.str();
                }
            };

            /// KeyBase specialisation when a key is pressed.
            class KeyPress : public KeyBase {
                public:
                /// Construct KeyPress instance forwarding the key object to KeyBase
                KeyPress(const Key& key) : KeyBase(key) {}

                EVENT_CLASS_TYPE(KeyPress)
            };

            /// KeyBase specialisation when a key is released.
            class KeyRelease : public KeyBase {
                public:
                /// Construct KeyRelease instance forwarding the key object to KeyBase
                KeyRelease(const Key& key) : KeyBase(key) {}

                EVENT_CLASS_TYPE(KeyRelease)
            };

            /// KeyBase specialisation when a key is typed (e.g. an input field)
            class KeyChar : public KeyBase {
                public:
                /// Constructs KeyChar instance by forewarding the key object to KeyBase
                KeyChar(const Key& key) : KeyBase(key) {}

                EVENT_CLASS_TYPE(KeyChar)

                std::string toString() const override {
                    std::stringstream ss;
                    ss << getName() << '(' << static_cast<uint32_t>(m_key.getKeyCode()) << ')';
                    return ss.str();
                }
            };

        }
    }
}

#endif
