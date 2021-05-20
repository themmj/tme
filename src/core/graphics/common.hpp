#ifndef _CORE_GRAPHICS_COMMON_H
#define _CORE_GRAPHICS_COMMON_H
/** @file */

#include "core/graphics/gl.hpp"
#include "core/log.hpp"
#include "core/storage.hpp"

namespace tme {
    namespace core {
        namespace graphics {

            /// clears previous OpenGL errors
            void clearGLErrors();

            /// log error from previously executed OpenGL function
            void logGLCall(const char* function, const char* file, uint32_t line);

            /**//**
             * Poll specific status of an OpenGL object and log errors if applicable.
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

            /// generate OpenGL function names for x to be used in logGLStatus
#           define GL_STATUS_FNS(x) glGet ## x ## iv, glGet ## x ## InfoLog

#           ifdef TME_DEBUG
                /// wrapper for OpenGL functions to log errors in debug mode
#               define glCall(x) clearGLErrors();\
                                 x;\
                                 logGLCall(#x, __FILE__, __LINE__)
#           else
#               define glCall(x) x
#           endif

            /// abstract class providing an interface for bindable objects
            class Bindable : public Mappable {
                protected:
                /// rendering id used to bind graphics object
                Identifier m_renderingId = 0;

                public:
                virtual ~Bindable() {}

                /// bind graphics object
                virtual void bind() const = 0;
                /// unbind graphics object
                virtual void unbind() const = 0;

                /// default bindable implementation, returning the rendering id assigned by OpenGL
                virtual Identifier getId() const override { return m_renderingId; }
            };

            /// interface for classes that are able to be rendered to the screen
            class Renderable {
                public:
                /// contains logic to render the object
                virtual void render() const = 0;
            };

        }
    }
}

#endif

