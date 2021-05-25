#ifndef _CORE_GRAPHICS_COMMON_H
#define _CORE_GRAPHICS_COMMON_H
/** @file */

#include "core/graphics/gl.hpp"
#include "core/log.hpp"
#include "core/storage.hpp"

namespace tme {
    namespace core {
        namespace graphics {

            /**//**
             * \brief Clears previous OpenGL errors.
             *
             * Clears OpenGL errors that have occurred until now. That way
             * one can check if the next OpenGL call caused an error.
             */
            void clearGLErrors();

            /**//**
             * \brief Log error from previously executed OpenGL function.
             *
             * Print an error and its source if one occurred.
             *
             * @param function char pointer to function name
             * @param file char pointer to file name
             * @param line line number
             */
            void logGLCall(const char* function, const char* file, uint32_t line);

            /**//**
             * \brief Poll specific status of an OpenGL object and log errors if applicable.
             *
             * Should be used in conjunction with the GL_STATUS_FNS macro for the first two parameter.
             *
             * @param getIvFn OpenGL function to poll one integer value
             * @param getInfoLogFn OpenGL function to get potential error logs
             * @param id identifier of the OpenGL object
             * @param status OpenGL enum value for the desired status
             *
             * @return true if no errors had to be logged, false otherwise
             */
            bool logGLStatus(void(*getIvFn)(GLuint, GLenum, GLint*), void(*getInfoLogFn)(GLuint, GLsizei, GLsizei*, GLchar*), GLuint id, GLenum status);

            /**//**
             * \brief Generate OpenGL function names for x to be used in logGLStatus.
             *
             * @param x name of the object for which the functions should be generated
             */
#           define GL_STATUS_FNS(x) glGet ## x ## iv, glGet ## x ## InfoLog

#           ifdef TME_DEBUG
                /**//**
                 * \brief Wrapper for OpenGL functions to log errors in debug mode.
                 *
                 * Will clear all OpenGL errors, then execute the function, then log an error if one occurred.
                 *
                 * @param x OpenGL function to be called
                 */
#               define glCall(x) clearGLErrors();\
                                 x;\
                                 logGLCall(#x, __FILE__, __LINE__)
#           else
#               define glCall(x) x
#           endif

            /**//**
             * \brief Interface providing an interface for bindable objects.
             *
             * Implements default behavior for Mappable returning the rendering id which is also used
             * for binding / unbinding.
             */
            class Bindable : public Mappable {
                protected:
                /// rendering id used to bind graphics object
                Identifier m_renderingId = 0;

                public:
                virtual ~Bindable() {}

                /**//**
                 * \brief Bind graphics object.
                 */
                virtual void bind() const = 0;
                /**//**
                 * \brief Unbind graphics object.
                 */
                virtual void unbind() const = 0;

                /**//**
                 * \brief Default Bindable implementation using the renderer id.
                 *
                 * @return the rendering id assigned by OpenGL
                 */
                virtual Identifier getId() const override { return m_renderingId; }
            };

            /**//**
             * \brief Interface for classes that are able to be rendered to the screen.
             */
            class Renderable {
                public:
                /**//**
                 * \brief Logic to render the object.
                 */
                virtual void render() = 0;
            };

            /// a single byte with the value 0, i.e. used to clear buffers
            constexpr unsigned char NULL_BYTE = 0;

        }
    }
}

#endif

