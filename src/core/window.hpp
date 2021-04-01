/** @file */
#ifndef _CORE_WINDOW_H
#define _CORE_WINDOW_H

#include <string>
#include "imgui.h"
#include "core/loggable.hpp"
#include "core/events/event.hpp"
#include "core/events/handler.hpp"
#include "core/map.hpp"

namespace tme {
    namespace core {

        /// Base class for windows.
        class Window : public Loggable, Mappable {
            using EventHandlerPtr = core::events::Handler*;

            Identifier m_id;

            protected:
            /// type alias for window dimension
            using Dimension = uint32_t;

            /// Imgui context
            ImGuiContext* m_imGuiContext;

            public:
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
                /// this pointer to access member in callback lambdas
                Window* m_this;
                /// timestamp of the last update
                double lastUpdate;

                /**//**
                 * \brief window data constructor
                 *
                 * @param eventHandler handler to which events by the window will be propagated to
                 * @param windowTitle
                 * @param windowWidth
                 * @param windowHeight
                 * @param vSync enable vsync of the window or not
                 */
                Data(EventHandlerPtr eventHandler, const std::string& windowTitle = "not set", Dimension windowWidth = 300, Dimension windowHeight = 300, bool vSync = true)
                    : title(windowTitle), width(windowWidth), height(windowHeight), vSyncEnabled(vSync), handler(eventHandler) {}
            };

            protected:
            /// window data
            Data m_data;

            /// construct new window base from data
            Window(const Data& data);

            public:
            /**//**
             * Static method to create a new window.
             * Implementation dependent on chosen platform inside /platform
             *
             * @param data parameters used to create window.
             * @return Owning pointer to created window.
             */
            static Window* create(const Data& data);
            virtual ~Window();

            /// initialize platform specific things to be able to open windows and render to them
            static void init();

            /// teardown initally set up platform specific aspects for window creation and rendering
            static void shutdown();

            /// Update method called every iteration of the application loop.
            virtual void onUpdate() = 0;

            /// set window as active window to render to it
            void setActive();

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
            void setTitle(const std::string& title);

            /**//**
             * \brief Set window vsync setting.
             *
             * @param enable desired vsync setting (true to enable, false to disable)
             */
            void setVSync(bool enable);

            virtual std::string toString() const override;

            /**//**
             * \brief Get relative x coordinate based on window dimension.
             *
             * @param absX absolute x value
             *
             * @return corresponding relative coordinate based on window width
             */
            template<typename T>
            double getRelativeX(T absX) const{
                return static_cast<double>(absX) / static_cast<double>(getWidth());
            }

            /**//**
             * \brief Get relative y coordinate based on window dimension.
             *
             * @param absY absolute y value
             *
             * @return corresponding relative coordinate based on window height
             */
            template<typename T>
            double getRelativeY(T absY) const {
                return static_cast<double>(absY) / static_cast<double>(getHeight());
            }

            Identifier getId() const override { return m_id; }

            protected:
            /// set window as current render target for specific platform
            virtual void setActiveInternal() = 0;

            /**//**
             * \brief Set window title for specific platform.
             *
             * @param title new window title
             */
            virtual void setTitleInternal(const std::string& title) = 0;

            /**//**
             * \brief Set window vsync setting for specific platform.
             *
             * @param enable desired vsync setting (true to enable, false to disable)
             */
            virtual void setVSyncInternal(bool enable) = 0;
        };

    }
}

#endif
