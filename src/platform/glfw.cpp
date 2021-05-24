/** @file */
#include <memory>
#include "core/graphics/gl.hpp"
#include "platform/glfw.hpp"
#include "core/log.hpp"
#include "core/window.hpp"
#include "core/events/window.hpp"
#include "core/events/key.hpp"
#include "core/events/mouse.hpp"

#include "imgui.h"
#include "backends/imgui_impl_opengl3.h"

namespace tme {

    namespace core {
        Window* Window::create(const Window::Data& data) {
            return new platform::GlfwWindow(data);
        }
    }


    namespace platform {

        /// code generation to get user data contained in the window instance and cast it accordingly
        #define GET_GLFW_DATA core::Window::Data& data = *static_cast<core::Window::Data*>(glfwGetWindowUserPointer(window))

        int32_t glfwToTmeKeyCode(int32_t glfwKeyCode) {
            auto result = glfwKeyMap.find(glfwKeyCode);
            if (result == glfwKeyMap.end()) {
                return TME_KEY_UNKNOWN;
            }
            return result->second;
        }

        std::unique_ptr<Context> Context::create() {
            return std::make_unique<GlfwContext>();
        }

        GlfwContext::GlfwContext() {
            glfwSetErrorCallback([](int error, const char* description){
                TME_ERROR("[GLFW] ({}): {}", error, description);
            });
            TME_ASSERT(glfwInit(), "failed to initialize glfw");
        }

        GlfwContext::~GlfwContext() {
            glfwTerminate();
            TME_INFO("terminated glfw");
        }

        GlfwWindow::GlfwWindow(const BaseWindow::Data& data) : BaseWindow(data){
            TME_TRACE("creating {}", *this);

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

            m_window = glfwCreateWindow(static_cast<int32_t>(m_data.width), static_cast<int32_t>(m_data.height), &m_data.title[0], nullptr, nullptr);
            if (!m_window) {
                // guarantees a debugger trap
                TME_ASSERT(m_window, "could not create window");
                // early exit to avoid seg faults
                return;
            }

            glfwMakeContextCurrent(m_window);
            glfwSetWindowUserPointer(m_window, &m_data);
            setVSync(m_data.vSyncEnabled);
            
            // load gl
            TME_ASSERT(gladLoadGLLoader((GLADloadproc) glfwGetProcAddress), "glad could not load opengl loader");

            setupCallbacks();

            m_data.lastUpdate = glfwGetTime();

            ImGui_ImplOpenGL3_Init("#version 330");

            TME_INFO("created {}", *this);
        }

        GlfwWindow::~GlfwWindow() {
            ImGui_ImplOpenGL3_Shutdown();
            glfwDestroyWindow(m_window);
            TME_INFO("destroyed {}", *this);
        }

        void GlfwWindow::update() {
            double currentUpdate = glfwGetTime();
            core::events::WindowUpdate updateEvent(currentUpdate - m_data.lastUpdate);
            m_data.lastUpdate = currentUpdate;
            m_data.handler->onEvent(updateEvent);
        }

        void GlfwWindow::swapBuffer() {
            glfwSwapBuffers(m_window);
        }

        void GlfwWindow::pollEvents() {
            glfwPollEvents();
        }

        void GlfwWindow::setTitleInternal(const std::string& title) {
            glfwSetWindowTitle(m_window, &title[0]);
        }

        void GlfwWindow::setVSyncInternal(bool enable) {
            if (enable)
                glfwSwapInterval(1);
            else
                glfwSwapInterval(0);
        }

        void GlfwWindow::setupCallbacks() {
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
                int frameBufferWidth, frameBufferHeight;
                glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

                core::events::WindowResize event(data.width, data.height, widthFactor, heightFactor, static_cast<uint32_t>(frameBufferWidth), static_cast<uint32_t>(frameBufferHeight));
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
                yPos = data.m_this->getHeight() - yPos;
                core::events::MouseMove event(xPos, yPos, data.m_this->getRelativeX(xPos), data.m_this->getRelativeY(yPos));
                data.handler->onEvent(event);
            });
            // GCOVR_EXCL_STOP
        }

    }
}

