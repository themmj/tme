#ifndef _CORE_GRAPHICS_COMMON_H
#define _CORE_GRAPHICS_COMMON_H
/** @file */

#include "core/graphics/gl.hpp"
#include "core/log.hpp"
#include "core/map.hpp"

namespace tme {
    namespace core {
        namespace graphics {

            /// clears previous OpenGL errors
            void clearGLErrors();

            /// log error from previously executed OpenGL function
            void logGLCall(const char* function, const char* file, uint32_t line);

            bool logGLStatus(void(*getIvFn)(GLuint, GLenum, GLint*), void(*getInfoLogFn)(GLuint, GLsizei, GLsizei*, GLchar*), GLuint id, GLenum status);

#           define GL_STATUS_FNS(x) glGet ## x ## iv, glGet ## x ## InfoLog

#           ifdef TME_DEBUG
                /// wrapper for OpenGL functions to log errors in debug mode
#               define glCall(x) clearGLErrors();\
                                 x;\
                                 logGLCall(#x, __FILE__, __LINE__)
#           else
#               define glCall(x) x
#           endif

            using RenderingId = uint32_t;

            class Bindable {
                protected:
                /// rendering id used to bind graphics object
                RenderingId m_renderingId = 0;

                public:
                virtual ~Bindable() {}

                /// bind graphics object
                virtual void bind() const = 0;
                /// unbind graphics object
                virtual void unbind() const = 0;
            };

        }
    }
}

#endif

