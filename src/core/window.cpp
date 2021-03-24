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

        void Window::setTitle(const std::string& title) {
            setTitleInternal(title);
            m_data.title = title;
        }

        void Window::setVSync(bool enable) {
            setVSyncInternal(enable);
            m_data.vSyncEnabled = enable;
        }

    }
}
