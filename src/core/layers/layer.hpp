#ifndef _CORE_LAYERS_LAYER_H
#define _CORE_LAYERS_LAYER_H
/** @file */

#include <memory>
#include <vector>
#include "core/events/event.hpp"
#include "core/exceptions/common.hpp"
#include "core/loggable.hpp"
#include "core/events/handler.hpp"
#include "core/graphics/common.hpp"

namespace tme {
    namespace core {
        namespace layers {

            /**//**
             * Base class for every layer implementing the Loggable
             * and events::Handler interface
             *
             * Derived classes should override the onEvent function
             * to process events and can override the toString
             * function to display extra information.
             */           
            class Layer : public Loggable, public events::Handler, public graphics::Renderable {
                std::string m_name;

                public:
                /// Construct Layer instance with name.
                Layer(const std::string& name) : m_name(name) {}
                virtual ~Layer() {}

                virtual std::string toString() const override { return m_name; }

                virtual void onEvent(events::Event&) override {}
            };

            /// Owning handle for a Layer
            using LayerHandle = std::unique_ptr<Layer>;

            /**//**
             * Stack structure to store Layer.
             *
             * Implements events::Handler to receive events and propagate
             * them to the layers, starting with the most recently added one.
             * If a layer successfully handled an event it will not be propagated
             * to the layers below it.
             */
            class Stack : public Loggable, public events::Handler, public graphics::Renderable {
                using Container = std::vector<LayerHandle>;
                Container m_layers;

                public:
                /// Construct a Layer Stack instance
                Stack();
                ~Stack();

                void onEvent(events::Event& event) override;
                void render() override;

                /**//**
                 * Construct a new Layer of type T in-place on the top
                 * of the Stack.
                 *
                 * @param args Arguments forwarded to the constructor of T
                 */
                template<typename T, typename... Args>
                void push(Args... args) {
                    m_layers.push_back(std::make_unique<T>(args...));
                }

                /// Removes top element of the stack.
                void pop();

                std::string toString() const override;
            };

        }
    }
}

#endif

