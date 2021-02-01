#ifndef _CORE_EVENTS_DISPATCHER_H
#define _CORE_EVENTS_DISPATCHER_H
/** @file */

#include "core/events/handler.hpp"
#include "core/events/event.hpp"

namespace tme {
    namespace core {

        /// Template to define member function pointer types taking a single argument
        template<typename ReturnType, typename Class, typename ArgType>
        using MemberFn = ReturnType(Class::*)(ArgType);

        namespace events {

            /// Event handling function type.
            template<typename Class, typename EventType>
            using EventFn = MemberFn<bool, Class, EventType&>;

            /**//**
             * \brief Base class for event dispatching
             *
             * Base class providing event handling interface.
             * Derive with class Class : public Dispatcher<Class> to access
             * member functions.
             * Includes Handler interface as dispatcher always handles events.
             */
            template<typename Class>
            class Dispatcher : public Handler {
                Class* obj;

                protected:
                /**//**
                 * Construct dispatcher instance.
                 *
                 * @param object Instance of Class on which the functions
                 *  in dispatchEvent is invoked.
                 */
                Dispatcher(Class* object) : obj(object) {}

                /**//**
                 * Invoke event handler function of instance obj
                 * with event of type EventType.
                 *
                 * @param event The event to be looked at.
                 * @param func The handler function pointer of instance obj.
                 * @return true if the handler was invoked. false otherwise.
                 */
                template<typename EventType>
                bool dispatchEvent(Event& event, EventFn<Class,EventType> func) {
                    if (event.getType() == EventType::getStaticType()) {
                        event.m_handled = ((*obj).*(func))(static_cast<EventType&>(event));
                        return true;
                    }
                    return false;
                }
            };

        }
    }
}

#endif
