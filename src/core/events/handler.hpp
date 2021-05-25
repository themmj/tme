/** @file */
#ifndef _CORE_EVENTS_HANDLER_H
#define _CORE_EVENTS_HANDLER_H

#include "core/events/event.hpp"

namespace tme {
    namespace core {
        namespace events {

            /**//**
             * \brief Interface providing onEvent method to handle events.
             */
            class Handler {
                public:
                /**//**
                 * Handle event.
                 *
                 * @param event the Event to be handled
                 */
                virtual void onEvent(Event& event) = 0;
            };

        }
    }
}

#endif
