#ifndef _CORE_EXCEPTIONS_INPUT_H
#define _CORE_EXCEPTIONS_INPUT_H
/** @file */

#include "core/exceptions/common.hpp"

namespace tme {
    namespace core {
        namespace exceptions {

            /// exception type for cases where invalid input was provided
            class InvalidInput final : public Base {
                public:
                /// construct InvalidInput instance with message msg
                InvalidInput(const char* msg) : Base("Invalid input", msg) {}
                ~InvalidInput() {}
            };

        }
    }
}

#endif

