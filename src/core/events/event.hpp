#ifndef EVENT_H
#define EVENT_H
/** @file */

#include <cstdint>
#include "core/loggable.hpp"

/// shift bit left by x
#define BIT(x) 1 << x

namespace tme {
    namespace core {
        namespace events {

            /// valid event types
            enum class Type {
                None = 0,
                WindowUpdate, WindowResize, WindowClose,
                KeyPress, KeyRelease, KeyChar,
                MouseKeyPress, MouseKeyRelease, MouseMove, MouseScroll
            };

            /// valid event categories
            enum Category : uint32_t {
                None = 0,
                Application = BIT(0),
                Input = BIT(1),
                Keyboard = BIT(2),
                Mouse = BIT(3)
            };

            /**//**
             * \brief Base class for events.
             *
             * Events should derive from this class to use them with
             * the Handler / Dispatcher.
             */
            class Event : public Loggable {
                template<typename> friend class Dispatcher;
                
                protected:
                /// status if the event has been processed by a handler
                bool m_handled = false;

                public:
                virtual ~Event() {}

                /**//**
                 * \brief Status of the event.
                 *
                 * @return true if a previous handler has processed the event, false otherwise
                 */
                inline bool isHandled() const { return m_handled; }

                /**//**
                 * \brief Get event type name.
                 *
                 * @return string version of event type
                 */
                virtual const char* getName() const = 0;
                
                /**//**
                 * \brief Get categories associated with event.
                 *
                 * @sa Category
                 *
                 * @return categories flags
                 */
                virtual Category getCategories() const = 0;
                
                /**//**
                 * \brief Get event type.
                 *
                 * @sa Type
                 *
                 * @return type of the Event
                 */
                virtual Type getType() const = 0;

                /**//**
                 * \brief Helper macro to generate override methods for type.
                 *
                 * @param type Event type to be used
                 */
                #define EVENT_CLASS_TYPE(type)  static inline Type getStaticType() { return Type::type; }\
                                                virtual Type getType() const override { return getStaticType(); }\
                                                virtual const char* getName() const override { return #type; }
                /**//**
                 * \brief Helper macro to generate override method for category
                 *
                 * @param category Event categories to be used
                 */
                #define EVENT_CLASS_CATEGORY(category) virtual Category getCategories() const override { return static_cast<Category>(category); }

                /**//**
                 * \brief Check if event is associated with category.
                 *
                 * @param category The category to check against
                 *
                 * @see Category
                 *
                 * @return true if event is associated with category, false otherwise
                 */
                inline bool isInCategory(Category category) const { return getCategories() & category; }

                virtual std::string toString() const override { return getName(); }
            };

        }
    }
}

#endif
