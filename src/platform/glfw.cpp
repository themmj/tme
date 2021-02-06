/** @file */
#include "platform/glfw.hpp"
#include "core/log.hpp"
#include "core/window.hpp"
#include "core/events/window.hpp"
#include "core/events/key.hpp"
#include "core/events/mouse.hpp"

namespace tme {

    core::Window* core::Window::create(const core::Window::Data& data) {
        return new platform::GlfwWindow(data);
    }

    namespace platform {

        bool GlfwWindow::s_glfwInitialized = false;

        /// code generation to get user data contained in the window instance and cast it accordingly
        #define GET_GLFW_DATA core::Window::Data& data = *static_cast<core::Window::Data*>(glfwGetWindowUserPointer(window))

        int32_t glfwToTmeKeyCode(int32_t glfwKeyCode) {
            auto result = glfwKeyMap.find(glfwKeyCode);
            if (result == glfwKeyMap.end()) {
                return TME_KEY_UNKNOWN;
            }
            return result->second;
        }

        void GlfwWindow::init() {
            TME_ASSERT(s_windowCount == 0, "cannot create more than one window");
            TME_TRACE("creating window {}", *this);
		    if (!s_glfwInitialized) {
			    TME_ASSERT(glfwInit(), "failed to initialize glfw");
			    s_glfwInitialized = true;
		    }

		    m_window = glfwCreateWindow(static_cast<int32_t>(m_data.width), static_cast<int32_t>(m_data.height), &m_data.title[0], nullptr, nullptr);
            if (!m_window) {
                TME_CRITICAL("could not create window");
                return;
            }

		    glfwMakeContextCurrent(m_window);
		    glfwSetWindowUserPointer(m_window, &m_data);
		    setVSync(m_data.vSyncEnabled);

		    // set glfw callbacks
            // the following callbacks are excluded from coverage tests as then cannot be tested with
            // automated unit tests because then i.e. depend on user input etc.
            // rest assured they are tested thoroughly in a manual fashion
            // GCOVR_EXCL_START
		    glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int32_t width, int32_t height) {
                GET_GLFW_DATA;
                auto widthFactor = data.m_this->getRelativeX(width);
                auto heightFactor = data.m_this->getRelativeY(height);
			    data.width = static_cast<Dimension>(width);
			    data.height = static_cast<Dimension>(height);

                core::events::WindowResize event(data.width, data.height, widthFactor, heightFactor);
                data.handler->onEvent(event);
		    });

		    glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
    			GET_GLFW_DATA;

                core::events::WindowClose event;
		    	data.handler->onEvent(event);
		    });
            
		    glfwSetKeyCallback(m_window, [](GLFWwindow* window, int32_t keyCode, int32_t /*scancode*/, int32_t action, int32_t mods) {
			    GET_GLFW_DATA;
                GlfwKey key(keyCode, mods);
			    switch (action) {
				    case GLFW_PRESS: {
                        core::events::KeyPress event(key);
					    data.handler->onEvent(event);
					    break;
				    }
				    case GLFW_RELEASE: {
                        core::events::KeyRelease event(key);
					    data.handler->onEvent(event);
					    break;
				    }
                    /* in case this is needed in the future
				    case GLFW_REPEAT: {
                        core::events::KeyRepeat event(key, ?);
					    data.handler->onEvent(event);
					    break;
				    }*/
			    }
		    });

		    glfwSetCharCallback(m_window, [](GLFWwindow* window, uint32_t codepoint) {
			    GET_GLFW_DATA;
                GlfwCharKey key(codepoint);
                core::events::KeyChar event(key);
			    data.handler->onEvent(event);
		    });

		    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int32_t button, int32_t action, int32_t mods) {
			    GET_GLFW_DATA;
                GlfwKey key(button, mods);
			    switch (action) {
				    case GLFW_PRESS: {
                        core::events::MouseKeyPress event(key);
					    data.handler->onEvent(event);
					    break;
				    }
				    case GLFW_RELEASE: {
                        core::events::MouseKeyRelease event(key);
					    data.handler->onEvent(event);
					    break;
				    }
			    }
		    });

		    glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset) {
			    GET_GLFW_DATA;
                core::events::MouseScroll event(xOffset, yOffset);
			    data.handler->onEvent(event);
		    });

		    glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos) {
			    GET_GLFW_DATA;
                core::events::MouseMove event(xPos, yPos, data.m_this->getRelativeX(xPos), data.m_this->getRelativeY(yPos));
			    data.handler->onEvent(event);
		    });
            // GCOVR_EXCL_STOP
            ++s_windowCount;
            TME_INFO("created window {}", *this);
        }

        void GlfwWindow::shutdown() {
            glfwDestroyWindow(m_window);
            TME_INFO("destroyed window {}", *this);
            // if it is the last window being destroyed
            // terminate glfw
            if (--s_windowCount == 0) {
                glfwTerminate();
                TME_INFO("terminated glfw");
            }
        }

        void GlfwWindow::onUpdate() {
            glfwPollEvents();
            glfwSwapBuffers(m_window);
        }

        void GlfwWindow::setTitle(const std::string& title) {
            m_data.title = title;
            glfwSetWindowTitle(m_window, &m_data.title[0]);
        }

        void GlfwWindow::setVSync(bool enable) {
            if (enable)
                glfwSwapInterval(1);
            else
                glfwSwapInterval(0);
            m_data.vSyncEnabled = enable;
        }

    }
}

