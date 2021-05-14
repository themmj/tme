#ifndef _CORE_EXCEPTIONS_COMMON_H
#define _CORE_EXCEPTIONS_COMMON_H
/** @file */

#include <exception>

namespace tme {
    namespace core {
        namespace exceptions {

            /**//**
             * Base class for TME exceptions.
             *
             * All TME related exceptions derive from this. That way the CATCH_ALL
             * macro can differentiate between TME and std exceptions.
             * Extends std::exceptions with a type function.
             */
            class Base : public std::exception {
                private:
                const char* m_type;
                const char* m_msg;
                public:
                /**//**
                 * Construct Base exception.
                 *
                 * @param type String representation of the exception type provided by derived type
                 * @param msg The message describing the error
                 */
                Base(const char* type, const char* msg) : m_type(type), m_msg(msg) {}
                virtual ~Base() = default;

                /// get string representation of the exception type
                const char* type() const noexcept { return m_type; }
                /// get message describing the error
                const char* what() const noexcept override { return m_msg; }
            };
            /// catch uncaught Base exceptions and std exceptions triggering an assert
#           define CATCH_ALL catch(const ::tme::core::exceptions::Base& e) {\
                TME_ASSERT(false, "unhandled TME exception: {}", e.what());\
            } catch(const std::exception& e) {\
                TME_ASSERT(false, "unhandled general exception: {}", e.what());\
            }
        
        }
    }
}

#endif

