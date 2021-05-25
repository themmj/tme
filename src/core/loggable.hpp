#ifndef _CORE_LOGGABLE_H
#define _CORE_LOGGABLE_H
/** @file */

#include <ostream>
#include <string>

namespace tme {
    namespace core {

        /**//**
         * \brief Base class providing toString method for stream operator.
         */
        class Loggable {
            public:
            /**//**
             * \brief Return string representation of object.
             *
             * @return string representation
             */
            virtual std::string toString() const = 0;
        };

        /**//**
         * \brief Adds Loggable::toString output to ostream.
         *
         * @param os the ostream to add the objects string representation
         * @param loggable object to write into ostream
         *
         * @return ostream with the string representation inside
         */
        inline std::ostream& operator<< (std::ostream& os, const Loggable& loggable) {
            return os << loggable.toString();
        }

    }
}

#endif
