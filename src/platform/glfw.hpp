/** @file */
#ifndef _PLATFORM_GLFW_H
#define _PLATFORM_GLFW_H

#include <unordered_map>
#include <GLFW/glfw3.h>
#include "core/key.hpp"
#include "core/window.hpp"

namespace tme {
    namespace platform {

        /// type alias for the datastructure used to map glfw keys to tme keys
        using GlfwKeyMap = std::unordered_map<int, const int>;
        /// key code mappings between glfw and tme
        const GlfwKeyMap glfwKeyMap = {
            { GLFW_KEY_UNKNOWN, TME_KEY_UNKNOWN },
            { GLFW_KEY_SPACE, TME_KEY_SPACE },
            { GLFW_KEY_APOSTROPHE, TME_KEY_APOSTROPHE },
            { GLFW_KEY_COMMA, TME_KEY_COMMA },
            { GLFW_KEY_MINUS, TME_KEY_MINUS },
            { GLFW_KEY_PERIOD, TME_KEY_PERIOD },
            { GLFW_KEY_SLASH, TME_KEY_SLASH },
            { GLFW_KEY_0, TME_KEY_0 },
            { GLFW_KEY_1, TME_KEY_1 },
            { GLFW_KEY_2, TME_KEY_2 },
            { GLFW_KEY_3, TME_KEY_3 },
            { GLFW_KEY_4, TME_KEY_4 },
            { GLFW_KEY_5, TME_KEY_5 },
            { GLFW_KEY_6, TME_KEY_6 },
            { GLFW_KEY_7, TME_KEY_7 },
            { GLFW_KEY_8, TME_KEY_8 },
            { GLFW_KEY_9, TME_KEY_9 },
            { GLFW_KEY_SEMICOLON, TME_KEY_SEMICOLON },
            { GLFW_KEY_EQUAL, TME_KEY_EQUAL },
            { GLFW_KEY_A, TME_KEY_A },
            { GLFW_KEY_B, TME_KEY_B },
            { GLFW_KEY_C, TME_KEY_C },
            { GLFW_KEY_D, TME_KEY_D },
            { GLFW_KEY_E, TME_KEY_E },
            { GLFW_KEY_F, TME_KEY_F },
            { GLFW_KEY_G, TME_KEY_G },
            { GLFW_KEY_H, TME_KEY_H },
            { GLFW_KEY_I, TME_KEY_I },
            { GLFW_KEY_J, TME_KEY_J },
            { GLFW_KEY_K, TME_KEY_K },
            { GLFW_KEY_L, TME_KEY_L },
            { GLFW_KEY_M, TME_KEY_M },
            { GLFW_KEY_N, TME_KEY_N },
            { GLFW_KEY_O, TME_KEY_O },
            { GLFW_KEY_P, TME_KEY_P },
            { GLFW_KEY_Q, TME_KEY_Q },
            { GLFW_KEY_R, TME_KEY_R },
            { GLFW_KEY_S, TME_KEY_S },
            { GLFW_KEY_T, TME_KEY_T },
            { GLFW_KEY_U, TME_KEY_U },
            { GLFW_KEY_V, TME_KEY_V },
            { GLFW_KEY_W, TME_KEY_W },
            { GLFW_KEY_X, TME_KEY_X },
            { GLFW_KEY_Y, TME_KEY_Y },
            { GLFW_KEY_Z, TME_KEY_Z },
            { GLFW_KEY_LEFT_BRACKET, TME_KEY_LEFT_BRACKET },
            { GLFW_KEY_BACKSLASH, TME_KEY_BACKSLASH },
            { GLFW_KEY_RIGHT_BRACKET, TME_KEY_RIGHT_BRACKET },
            { GLFW_KEY_GRAVE_ACCENT, TME_KEY_GRAVE_ACCENT },
            { GLFW_KEY_ESCAPE, TME_KEY_ESCAPE },
            { GLFW_KEY_ENTER, TME_KEY_ENTER },
            { GLFW_KEY_TAB, TME_KEY_TAB },
            { GLFW_KEY_BACKSPACE, TME_KEY_BACKSPACE },
            { GLFW_KEY_INSERT, TME_KEY_INSERT },
            { GLFW_KEY_DELETE, TME_KEY_DELETE },
            { GLFW_KEY_RIGHT, TME_KEY_RIGHT },
            { GLFW_KEY_LEFT, TME_KEY_LEFT },
            { GLFW_KEY_DOWN, TME_KEY_DOWN },
            { GLFW_KEY_UP, TME_KEY_UP },
            { GLFW_KEY_PAGE_UP, TME_KEY_PAGE_UP },
            { GLFW_KEY_PAGE_DOWN, TME_KEY_PAGE_DOWN },
            { GLFW_KEY_HOME, TME_KEY_HOME },
            { GLFW_KEY_END, TME_KEY_END },
            { GLFW_KEY_CAPS_LOCK, TME_KEY_CAPS_LOCK },
            { GLFW_KEY_NUM_LOCK, TME_KEY_NUM_LOCK },
            { GLFW_KEY_F1, TME_KEY_F1 },
            { GLFW_KEY_F2, TME_KEY_F2 },
            { GLFW_KEY_F3, TME_KEY_F3 },
            { GLFW_KEY_F4, TME_KEY_F4 },
            { GLFW_KEY_F5, TME_KEY_F5 },
            { GLFW_KEY_F6, TME_KEY_F6 },
            { GLFW_KEY_F7, TME_KEY_F7 },
            { GLFW_KEY_F8, TME_KEY_F8 },
            { GLFW_KEY_F9, TME_KEY_F9 },
            { GLFW_KEY_F10, TME_KEY_F10 },
            { GLFW_KEY_F11, TME_KEY_F11 },
            { GLFW_KEY_F12, TME_KEY_F12 },
            { GLFW_KEY_F13, TME_KEY_F13 },
            { GLFW_KEY_F14, TME_KEY_F14 },
            { GLFW_KEY_F15, TME_KEY_F15 },
            { GLFW_KEY_F16, TME_KEY_F16 },
            { GLFW_KEY_F17, TME_KEY_F17 },
            { GLFW_KEY_F18, TME_KEY_F18 },
            { GLFW_KEY_F19, TME_KEY_F19 },
            { GLFW_KEY_F20, TME_KEY_F20 },
            { GLFW_KEY_F21, TME_KEY_F21 },
            { GLFW_KEY_F22, TME_KEY_F22 },
            { GLFW_KEY_F23, TME_KEY_F23 },
            { GLFW_KEY_F24, TME_KEY_F24 },
            { GLFW_KEY_F25, TME_KEY_F25 },
            { GLFW_KEY_KP_0, TME_KEY_KP_0 },
            { GLFW_KEY_KP_1, TME_KEY_KP_1 },
            { GLFW_KEY_KP_2, TME_KEY_KP_2 },
            { GLFW_KEY_KP_3, TME_KEY_KP_3 },
            { GLFW_KEY_KP_4, TME_KEY_KP_4 },
            { GLFW_KEY_KP_5, TME_KEY_KP_5 },
            { GLFW_KEY_KP_6, TME_KEY_KP_6 },
            { GLFW_KEY_KP_7, TME_KEY_KP_7 },
            { GLFW_KEY_KP_8, TME_KEY_KP_8 },
            { GLFW_KEY_KP_9, TME_KEY_KP_9 },
            { GLFW_KEY_KP_DECIMAL, TME_KEY_KP_DECIMAL },
            { GLFW_KEY_KP_DIVIDE, TME_KEY_KP_DIVIDE },
            { GLFW_KEY_KP_MULTIPLY, TME_KEY_KP_MULTIPLY },
            { GLFW_KEY_KP_SUBTRACT, TME_KEY_KP_SUBTRACT },
            { GLFW_KEY_KP_ADD, TME_KEY_KP_ADD },
            { GLFW_KEY_KP_ENTER, TME_KEY_KP_ENTER },
            { GLFW_KEY_KP_EQUAL, TME_KEY_KP_EQUAL },
            { GLFW_KEY_LEFT_SHIFT, TME_KEY_LEFT_SHIFT },
            { GLFW_KEY_LEFT_CONTROL, TME_KEY_LEFT_CONTROL },
            { GLFW_KEY_LEFT_ALT, TME_KEY_LEFT_ALT },
            { GLFW_KEY_LEFT_SUPER, TME_KEY_LEFT_SUPER },
            { GLFW_KEY_RIGHT_SHIFT, TME_KEY_RIGHT_SHIFT },
            { GLFW_KEY_RIGHT_CONTROL, TME_KEY_RIGHT_CONTROL },
            { GLFW_KEY_RIGHT_ALT, TME_KEY_RIGHT_ALT },
            { GLFW_KEY_RIGHT_SUPER, TME_KEY_RIGHT_SUPER },
        };

        /**//**
         * \brief Conversion helper between glfw and tme keycodes.
         * Will translate unknown keys to TME_KEY_UNKNOWN.
         *
         * @param glfwKeyCode key code from glfw to map
         *
         * @return tme key code
         */
        int glfwToTmeKeyCode(int glfwKeyCode);

        /**//**
         * \brief Key implementation for glfw.
         */
        class GlfwKey : public core::Key {
            int m_modMask;

            public:
            /**//**
             * \brief Construct GlfwKey instance.
             *
             * @param keyCode the glfw keycode to be used (will be converted during construction)
             * @param modMask the glfw modmask provided in the event callback
             */
            GlfwKey(int keyCode, int modMask) : Key(glfwToTmeKeyCode(keyCode)), m_modMask(modMask) {}

            bool hasModShift() const override {
                return m_modMask & GLFW_MOD_SHIFT;
            }
            bool hasModControl() const override {
                return m_modMask & GLFW_MOD_CONTROL;
            }
            bool hasModAlt() const override {
                return m_modMask & GLFW_MOD_ALT;
            }
            bool hasModSuper() const override {
                return m_modMask & GLFW_MOD_SUPER;
            }
            bool hasModCapsLock() const override {
                return m_modMask & GLFW_MOD_CAPS_LOCK;
            }
            bool hasModNumLock() const override {
                return m_modMask & GLFW_MOD_NUM_LOCK;
            }
        };

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
