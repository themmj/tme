#ifndef _NAMESPACES_H
#define _NAMESPACES_H
/** @file */

/// TME
namespace tme {

    /// Essentials like logging, events etc.
    namespace core {

        /// Eventsystem including Dispatcher and base Event types.
        namespace events {}

        /// Layer system to build up application logic and handle events.
        namespace layers {}

    }

    /// Platform specific specialisation of core components.
    namespace platform{}

}


#endif
