#ifndef _CORE_GRAPHICS_INDEX_H
#define _CORE_GRAPHICS_INDEX_H
/** @file */

#include <sstream>
#include "core/graphics/buffer.hpp"

namespace tme {
    namespace core {
        namespace graphics {

            /**//**
             * Buffer implementation for an index buffer.
             *
             * Added functionality to get the amount of raw primitives inside the buffer for rendering.
             */
            class IndexBuffer final : public Buffer {
                GLsizei m_primitiveCount;
                public:
                /**//**
                 * Construct Buffer instance for GL_ELEMENT_ARRAY_BUFFER.
                 *
                 * Precalculate amount of primitives stored inside the buffer.
                 */
                IndexBuffer(GLsizeiptr primitivesPerEntry, GLsizeiptr entrySize, GLsizeiptr size);
                ~IndexBuffer();

                std::string toString() const override;
                /// get number of primitives stored inside the buffer (not just populated but all)
                inline GLsizei getPrimitiveCount() const { return m_primitiveCount;  }
            };

        }
    }
}

#endif

