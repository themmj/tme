#ifndef _CORE_EXCEPTIONS_INPUT_H
#define _CORE_EXCEPTIONS_INPUT_H
/** @file */

#include "core/exceptions/common.hpp"

namespace tme {
    namespace core {
        namespace exceptions {

            /**//**
             * \brief Exception type for cases where invalid input was provided.
             *
             * That includes anything from invalid file paths to invalid Identifier
             * for Storage access.
             */
            class InvalidInput final : public Base {
                public:
                /**//**
                 * \brief Construct InvalidInput instance with message msg.
                 *
                 * @param msg char pointer to message string
                 */
                InvalidInput(const char* msg) : Base("Invalid input", msg) {}
                ~InvalidInput() {}
            };

        }
    }
}

#endif

