/** @file */
#include "core/graphics/index.hpp"
#include <sstream>
#include "core/storage.hpp"

namespace tme {
    namespace  core {
        namespace graphics {

            IndexBuffer::IndexBuffer(GLsizeiptr primitivesPerEntry, GLsizeiptr entrySize, GLsizeiptr size)
                : Buffer(GL_ELEMENT_ARRAY_BUFFER, entrySize, size),
                m_primitiveCount(static_cast<GLsizei>(getSize() * primitivesPerEntry)) {}
            IndexBuffer::~IndexBuffer() {}

            std::string IndexBuffer::toString() const {
                std::stringstream ss;
                ss << "IndexBuffer(" << getId() << ',' << getSize() << ',' << getFreeSpace() << ')';
                return ss.str();
            }

        }
    }
}
