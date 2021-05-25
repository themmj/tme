/** @file */
#ifndef _CORE_KEY_H
#define _CORE_KEY_H

#include <sstream>
#include "core/loggable.hpp"

namespace tme {
    namespace core {

        /**//**
         * \brief Key base class.
         *
         * Base class from which platform specific implementation can be derived.
         * Provides comparison functions and getters for the key code
         * and access to the applied modifiers.
         */
        class Key : public Loggable {
            int32_t m_keyCode;
            public:
            /**//**
             * \brief Construct Key instance from tme key code.
             *
             * @param keyCode the code of the key for which the Key should be created
             */
            Key(int32_t keyCode) : m_keyCode(keyCode) {}
            virtual ~Key() {}

            /**//**
             * \brief Determine if key is a specific one.
             *
             * @param keyCode the tme key code to be compared to.
             *
             * @return true if the provided key code matches, false otherwise
             */
            bool isKey(int32_t keyCode) const { return m_keyCode == keyCode; }
            /**//**
             * \brief Get the tme key code.
             *
             * @return key code of the key
             */
            int32_t getKeyCode() const { return m_keyCode; }

            /**//**
             * \brief Check if key has mod shift
             *
             * @return true if the shift mod was active
             */
            virtual bool hasModShift() const = 0;
            /**//**
             * \brief Check if key has mod control
             *
             * @return true if the control mod was active
             */
            virtual bool hasModControl() const = 0;
            /**//**
             * \brief Check if key has mod alt
             *
             * @return true if the alt mod was active
             */
            virtual bool hasModAlt() const = 0;
            /**//**
             * \brief Check if key has mod super
             *
             * @return true if the super mod was active
             */
            virtual bool hasModSuper() const = 0;
            /**//**
             * \brief Check if key has mod caps lock
             *
             * @return true if the caps lock mod was active
             */
            virtual bool hasModCapsLock() const = 0;
            /**//**
             * \brief Check if key has mod num lock.
             *
             * @return true if the num lock mod was active
             */
            virtual bool hasModNumLock() const = 0;

            std::string toString() const override {
                std::stringstream ss;
                ss << "Key(" << m_keyCode << ')';
                return ss.str();
            }
        };

    }
}

// tme keycodes
// @cond KEY_CODES_INCLUDED
// (to exclude them from the documentation)
#define TME_KEY_UNKNOWN  -1
#define TME_MOUSE_BUTTON_LEFT 0
#define TME_MOUSE_BUTTON_RIGHT 1
#define TME_MOUSE_BUTTON_MIDDLE 2
#define TME_KEY_SPACE  3
#define TME_KEY_APOSTROPHE  114
#define TME_KEY_COMMA  115
#define TME_KEY_MINUS  116
#define TME_KEY_PERIOD  4
#define TME_KEY_SLASH  5
#define TME_KEY_0  6
#define TME_KEY_1  7
#define TME_KEY_2  8
#define TME_KEY_3  9
#define TME_KEY_4  10
#define TME_KEY_5  11
#define TME_KEY_6  12
#define TME_KEY_7  13
#define TME_KEY_8  14
#define TME_KEY_9  15
#define TME_KEY_SEMICOLON  16
#define TME_KEY_EQUAL  17
#define TME_KEY_A  18
#define TME_KEY_B  19
#define TME_KEY_C  20
#define TME_KEY_D  21
#define TME_KEY_E  22
#define TME_KEY_F  23
#define TME_KEY_G  24
#define TME_KEY_H  25
#define TME_KEY_I  26
#define TME_KEY_J  27
#define TME_KEY_K  28
#define TME_KEY_L  29
#define TME_KEY_M  30
#define TME_KEY_N  31
#define TME_KEY_O  32
#define TME_KEY_P  33
#define TME_KEY_Q  34
#define TME_KEY_R  35
#define TME_KEY_S  36
#define TME_KEY_T  37
#define TME_KEY_U  38
#define TME_KEY_V  39
#define TME_KEY_W  40
#define TME_KEY_X  41
#define TME_KEY_Y  42
#define TME_KEY_Z  43
#define TME_KEY_LEFT_BRACKET  44
#define TME_KEY_BACKSLASH  45
#define TME_KEY_RIGHT_BRACKET  46
#define TME_KEY_GRAVE_ACCENT  47
#define TME_KEY_ESCAPE  48
#define TME_KEY_ENTER  49
#define TME_KEY_TAB  50
#define TME_KEY_BACKSPACE  51
#define TME_KEY_INSERT  52
#define TME_KEY_DELETE  53
#define TME_KEY_RIGHT  54
#define TME_KEY_LEFT  55
#define TME_KEY_DOWN  56
#define TME_KEY_UP  57
#define TME_KEY_PAGE_UP  58
#define TME_KEY_PAGE_DOWN  59
#define TME_KEY_HOME  60
#define TME_KEY_END  61
#define TME_KEY_CAPS_LOCK  62
#define TME_KEY_NUM_LOCK  63
#define TME_KEY_F1  64
#define TME_KEY_F2  65
#define TME_KEY_F3  66
#define TME_KEY_F4  67
#define TME_KEY_F5  68
#define TME_KEY_F6  69
#define TME_KEY_F7  70
#define TME_KEY_F8  71
#define TME_KEY_F9  72
#define TME_KEY_F10  73
#define TME_KEY_F11  74
#define TME_KEY_F12  75
#define TME_KEY_F13  76
#define TME_KEY_F14  77
#define TME_KEY_F15  78
#define TME_KEY_F16  79
#define TME_KEY_F17  80
#define TME_KEY_F18  81
#define TME_KEY_F19  82
#define TME_KEY_F20  83
#define TME_KEY_F21  84
#define TME_KEY_F22  85
#define TME_KEY_F23  86
#define TME_KEY_F24  87
#define TME_KEY_F25  88
#define TME_KEY_KP_0  89
#define TME_KEY_KP_1  90
#define TME_KEY_KP_2  91
#define TME_KEY_KP_3  92
#define TME_KEY_KP_4  93
#define TME_KEY_KP_5  94
#define TME_KEY_KP_6  95
#define TME_KEY_KP_7  96
#define TME_KEY_KP_8  97
#define TME_KEY_KP_9  98
#define TME_KEY_KP_DECIMAL  99
#define TME_KEY_KP_DIVIDE  100
#define TME_KEY_KP_MULTIPLY  101
#define TME_KEY_KP_SUBTRACT  102
#define TME_KEY_KP_ADD  103
#define TME_KEY_KP_ENTER  104
#define TME_KEY_KP_EQUAL  105
#define TME_KEY_LEFT_SHIFT  106
#define TME_KEY_LEFT_CONTROL  107
#define TME_KEY_LEFT_ALT  108
#define TME_KEY_LEFT_SUPER  109
#define TME_KEY_RIGHT_SHIFT  110
#define TME_KEY_RIGHT_CONTROL  111
#define TME_KEY_RIGHT_ALT  112
#define TME_KEY_RIGHT_SUPER  113
// @endcond

#endif
