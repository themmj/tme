#ifndef _CORE_GRAPHICS_INDEX_H
#define _CORE_GRAPHICS_INDEX_H
/** @file */

#include <sstream>
#include "core/graphics/buffer.hpp"

namespace tme {
    namespace core {
        namespace graphics {

            /**//**
             * \brief Buffer implementation for an index buffer.
             *
             * Additional functionality to get the amount of raw primitives inside the buffer for rendering.
             */
            class IndexBuffer final : public Buffer {
                GLsizei m_primitiveCount;
                public:
                /**//**
                 * \brief Construct Buffer instance for indices.
                 *
                 * Uses GL_ELEMENT_ARRAY_BUFFER for its type.
                 * Precalculates amount of primitives stored inside the buffer.
                 *
                 * @param primitivesPerEntry number of primitives inside a single index entry
                 * @param entrySize size of a single index entry in bytes
                 * @param size number of index entries the buffer should be able to hold 
                 */
                IndexBuffer(GLsizeiptr primitivesPerEntry, GLsizeiptr entrySize, GLsizeiptr size);
                ~IndexBuffer();

                std::string toString() const override;
                /**//**
                 * \brief Get number of primitives stored inside the buffer.
                 *
                 * Meaning all possible primitives, not just populated ones.
                 *
                 * @return total number of primitives
                 */
                inline GLsizei getPrimitiveCount() const { return m_primitiveCount;  }
            };

        }
    }
}

#endif

