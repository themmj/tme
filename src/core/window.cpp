/** @file */
#include "core/window.hpp"
#include <sstream>

namespace tme {
    namespace core {

        uint32_t Window::s_windowCount = 0;


        std::string Window::toString() const {
            std::stringstream ss;
            ss << "Window: " << getTitle() << '(' << getWidth() << ',' << getHeight() << ',' << isVSync() << ')';
            return ss.str();
        }

    }
}
