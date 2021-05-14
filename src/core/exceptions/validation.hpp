#ifndef _CORE_EXCEPTIONS_VALIDATION_H
#define _CORE_EXCEPTIONS_VALIDATION_H
/** @file */

#include "core/exceptions/common.hpp"

namespace tme {
    namespace core {
        namespace exceptions {

            /// exception type for cases where a syntax error is detected during validation
            class SyntaxError final : public Base {
                public:
                /// construct SyntaxError instance with message msg
                SyntaxError(const char* msg) : Base("Syntax error", msg) {}
                ~SyntaxError() {}
            };

            /// exception type for cases where a linking error is detected during validation
            class LinkingError final : public Base {
                public:
                /// construct LinkingError instance with message msg
                LinkingError(const char* msg) : Base("Linking error", msg) {}
                ~LinkingError() {}
            };

            /// exception type for cases where a validation error is detected
            class ValidationError final : public Base {
                public:
                /// construct ValidationError instance with message msg
                ValidationError(const char* msg) : Base("Validation error", msg) {}
                ~ValidationError() {}
            };

        }
    }
}

#endif

