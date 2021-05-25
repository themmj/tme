#ifndef _CORE_EXCEPTIONS_GRAPHICS_H
#define _CORE_EXCEPTIONS_GRAPHICS_H
/** @file */

#include "core/exceptions/common.hpp"

namespace tme {
    namespace core {
        namespace exceptions {

            /**//**
             * \brief Exception type for cases where a Buffer does not have enough space to hold data.
             */
            class InsufficientBufferSpace final : public Base {
                public:
                /**//**
                 * \brief Construct InsufficientBufferSpace instance with message msg.
                 *
                 * @param msg char pointer to message string
                 */
                InsufficientBufferSpace(const char* msg) : Base("Insufficient buffer space", msg) {}
                ~InsufficientBufferSpace() {}
            };

        }
    }
}

#endif

