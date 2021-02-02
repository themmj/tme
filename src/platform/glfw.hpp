#ifndef _PLATFORM_GLFW_H
#define _PLATFORM_GLFW_H

#include <GLFW/glfw3.h>
#include "core/window.hpp"

namespace tme {
    namespace platform {

        /**//**
         * \brief Window implementation using GLFW.
         */
        class GlfwWindow : public core::Window {
            using BaseWindow = core::Window;

            GLFWwindow* m_window;

            void init();
            void shutdown();

            public:
            /// state information if glfw has been initialized
            static bool s_glfwInitialized;
            /// construct window instance using glfw from data
            GlfwWindow(const BaseWindow::Data& data) : BaseWindow(data) { init(); }
            ~GlfwWindow() { shutdown(); }

            void onUpdate() override;
            void setTitle(const std::string& title) override;
            void setVSync(bool enable) override; 
        };

    }
}

#endif
