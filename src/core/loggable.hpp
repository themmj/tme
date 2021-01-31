#ifndef _CORE_LOGGABLE_H
#define _CORE_LOGGABLE_H
/** @file */

#include <ostream>
#include <string>

namespace tme {
    namespace core {

        /**//**
         * Base class providing toString method for
         * stream operator.
         */
        class Loggable {
            public:
            /**//**
             * Return string representation of object.
             */
            virtual std::string toString() const = 0;
        };

        /**//**
         * Add Loggable::toString output to ostream.
         */
        inline std::ostream& operator<< (std::ostream& os, const Loggable& loggable) {
            return os << loggable.toString();
        }

    }
}

#endif
