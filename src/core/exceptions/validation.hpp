#ifndef _CORE_EXCEPTIONS_VALIDATION_H
#define _CORE_EXCEPTIONS_VALIDATION_H
/** @file */

#include "core/exceptions/common.hpp"

namespace tme {
    namespace core {
        namespace exceptions {

            /**//**
             * \brief Exception type for cases where a syntax error is detected during validation.
             */
            class SyntaxError final : public Base {
                public:
                /**//**
                 * \brief Construct SyntaxError instance with message msg.
                 *
                 * @param msg char pointer to message string
                 */
                SyntaxError(const char* msg) : Base("Syntax error", msg) {}
                ~SyntaxError() {}
            };

            /**//**
             * \brief Exception type for cases where a linking error is detected during validation.
             */
            class LinkingError final : public Base {
                public:
                /**//**
                 * \brief Construct LinkingError instance with message msg.
                 *
                 * @param msg char pointer to message string
                 */
                LinkingError(const char* msg) : Base("Linking error", msg) {}
                ~LinkingError() {}
            };

            /**//**
             * \brief Exception type for cases where a validation error is detected.
             */
            class ValidationError final : public Base {
                public:
                /**//**
                 * \brief Construct ValidationError instance with message msg.
                 *
                 * @param msg char pointer to message string
                 */
                ValidationError(const char* msg) : Base("Validation error", msg) {}
                ~ValidationError() {}
            };

        }
    }
}

#endif

