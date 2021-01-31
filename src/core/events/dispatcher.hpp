#ifndef _CORE_EVENTS_DISPATCHER_H
#define _CORE_EVENTS_DISPATCHER_H
/** @file */

#include "core/events/event.hpp"

namespace tme {
    namespace core {
        namespace events {

            /// Event handling function type.
            template<typename Class, typename EventType>
            using EventFn = bool(Class::*)(EventType&);

            /**//**
             * \brief Base class for event dispatching
             *
             * Base class providing event handling interface.
             * Derive with class Class : public Dispatcher<Class> to access
             * member functions.
             * Provides abstract onEvent method.
             */
            template<typename Class>
            class Dispatcher {
                protected:
                /**//**
                 * Invoke event handler function of instance obj
                 * with event of type EventType.
                 *
                 * @param event The event to be looked at.
                 * @param obj The instance of which the handler should be invoked.
                 * @param func The handler function pointer of instance obj.
                 * @return true if the handler was invoked. false otherwise.
                 */
                template<typename EventType>
                bool dispatchEvent(Event& event, Class* obj, EventFn<Class,EventType> func) {
                    if (event.getType() == EventType::getStaticType()) {
                        event.m_handled = ((*obj).*(func))(static_cast<EventType&>(event));
                        return true;
                    }
                    return false;
                }

                public:
                /**//**
                 * Handle event.
                 *
                 * @param event The event to be handled.
                 */
                virtual void onEvent(Event& event) = 0;
            };

        }
    }
}

#endif
