/** @file */
#ifndef _CORE_WINDOW_H
#define _CORE_WINDOW_H

#include <string>
#include "core/loggable.hpp"
#include "core/events/event.hpp"
#include "core/events/handler.hpp"

namespace tme {
    namespace core {

        /// Base class for windows.
        class Window : public Loggable {
            using EventHandlerPtr = core::events::Handler*;

            protected:
            /// type alias for window dimension
            using Dimension = uint32_t;

            public:
            /// Global window counter. Needs to be in-/decremented in the derived classes.
            static uint32_t s_windowCount;
            /// window data container
            struct Data {
                /// window title
                std::string title;
                /// window width in pixels
                Dimension width;
                /// window height in pixels
                Dimension height;
                /// is vsync enabled
                bool vSyncEnabled;
                /// handler to propagate events to
                EventHandlerPtr handler;

                /**//**
                 * \brief window data constructor
                 *
                 * @param windowTitle
                 * @param windowWidth
                 * @param windowHeight
                 * @param eventHandler handler to which events by the window will be propagated to
                 * @param vSync enable vsync of the window or not
                 */
                Data(const std::string& windowTitle, Dimension windowWidth, Dimension windowHeight, EventHandlerPtr eventHandler, bool vSync = 0) 
                    : title(windowTitle), width(windowWidth), height(windowHeight), vSyncEnabled(vSync), handler(eventHandler) {}
            };

            protected:
            /// window data
            Data m_data;

            public:
            /**//**
             * Static method to create a new window.
             * Implementation dependent on chosen platform inside /platform
             *
             * @param data parameters used to create window.
             * @return Owning pointer to created window.
             */
            static Window* create(const Data& data);

            /// construct new window base from data
            Window(const Data& data) : m_data(data) {}
            virtual ~Window() {}

            /// Update method called every iteration of the application loop.
            virtual void onUpdate() = 0;

            /**//**
             * \brief Get window width.
             *
             * @return current window width
             */
            inline Dimension getWidth() const { return m_data.width; }
            /**//**
             * \brief Get window height.
             *
             * @return current window height
             */
            inline Dimension getHeight() const { return m_data.height; }
            /**//**
             * \brief Get window vsync setting.
             *
             * @return true of vsync is enabled. false otherwise
             */
            inline bool isVSync() const { return m_data.vSyncEnabled; }
            /**//**
             * \brief Get window title.
             *
             * @return current window title
             */
            inline std::string getTitle() const { return m_data.title; }

            /**//**
             * \brief Set window title.
             *
             * @param title new window title
             */
            virtual void setTitle(const std::string& title) = 0;
            /**//**
             * \brief Set window vsync setting.
             *
             * @param enable desired vsync setting (true to enable, false to diable)
             */
            virtual void setVSync(bool enable) = 0;

            virtual std::string toString() const override;
        };

    }
}

#endif
