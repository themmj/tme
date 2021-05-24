/** @file */

#include "core/layers/layer.hpp"
#include <stdexcept>
#include <sstream>
#include "core/exceptions/input.hpp"

namespace tme {
    namespace core {
        namespace layers {

            Stack::Stack() : m_layers() {}
            Stack::~Stack() {}

            void Stack::onEvent(events::Event& event) {
                for (auto it = m_layers.crbegin(); it != m_layers.crend(); ++it) {
                    (*it)->onEvent(event);
                    if (event.isHandled())
                        break;
                }
            }

            void Stack::render() {
                for (auto it = m_layers.cbegin(); it != m_layers.cend(); ++it) {
                    (*it)->render();
                }
            }

            void Stack::pop() {
                TME_ASSERT(m_layers.size() != 0, "pop should not be called on an empty stack");
                m_layers.pop_back();
            }

            std::string Stack::toString() const {
                std::stringstream ss;
                ss << "LayerStack( ";
                for (auto it = m_layers.crbegin(); it != m_layers.crend(); ++it) {
                    ss << (*it)->toString() << ' ';
                }
                ss << ')';
                return ss.str();
            }

        }
    }
}

