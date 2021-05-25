/** @file */

#include "core/graphics/common.hpp"

namespace tme {
    namespace core {
        namespace graphics {

            void clearGLErrors() {
                while (glGetError() != GL_NO_ERROR);
            }

            void logGLCall(const char* function, const char* file, uint32_t line) {
                if (GLenum error = glGetError()) {
                    TME_ERROR("[OpenGL] ({}) from {} in {}:{}", error, function, file, line);
                }
            }

            bool logGLStatus(void(*getIvFn)(GLuint, GLenum, GLint*), void(*getInfoLogFn)(GLuint, GLsizei, GLsizei*, GLchar*), GLuint id, GLenum status) {
                int result;
                glCall(getIvFn(id, status, &result));
                if (result != GL_TRUE) {
                    int length;
                    glCall(getIvFn(id, GL_INFO_LOG_LENGTH, &length));
                    char* message = new char[static_cast<unsigned long>(length)];
                    glCall(getInfoLogFn(id, length, &length, message));
                    TME_ERROR("[OpenGL] {}", message);
                    delete[] message;
                    return false;
                }
                return true;
            }

        }
    }
}

