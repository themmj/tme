#ifndef _NAMESPACES_H
#define _NAMESPACES_H
/** @file */

/// TME
namespace tme {

    /// Application logic.
    namespace app {

        /// Application graphics like Batchable objects.
        namespace graphics {}

        /// Layer implementation to build up applications.
        namespace layers {}

    }

    /// Essentials like logging, events etc.
    namespace core {

        /// Eventsystem including Dispatcher and base Event types.
        namespace events {}

        /// Custom exceptions types and functionality.
        namespace exceptions {}

        /// Layer system to build up application logic and handle events.
        namespace layers {}

        /// Graphics pipeline
        namespace graphics {}

    }

    /// Platform specific specialisation of core components.
    namespace platform{}

}


#endif
