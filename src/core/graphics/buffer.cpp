/** @file */
#include "core/graphics/buffer.hpp"
#include <sstream>
#include "core/storage.hpp"

namespace tme {
    namespace  core {
        namespace graphics {

            Buffer::Buffer(GLenum type, GLsizeiptr entrySize, GLsizeiptr size)
                : m_type(type),
                m_entrySize(entrySize),
                m_size(size),
                m_nextOffset(0),
                m_freeSpaces() {
                glCall(glGenBuffers(1, &m_renderingId));
                bind();
                glCall(glBufferData(type, entrySize * size, NULL, GL_STATIC_DRAW));
            }
            Buffer::~Buffer() {
                unbind();
                glCall(glDeleteBuffers(1, &m_renderingId));
            }

            Buffer::Space Buffer::add(GLsizeiptr size, const void* data) {
                bind();
                for (auto iter = m_freeSpaces.begin(); iter != m_freeSpaces.end(); ++iter) {
                    if (iter->size == size) {
                        GLsizeiptr offset = iter->offset;
                        glCall(glBufferSubData(m_type, offset * m_entrySize, size * m_entrySize, data));
                        m_freeSpaces.erase(iter);
                        return { offset, size };
                    }
                }
                // subtracting here avoids overflow
                if (m_size - size >= m_nextOffset) {
                    Space dataSpace{ m_nextOffset, size };
                    update(dataSpace, data);
                    m_nextOffset += size;
                    return dataSpace;
                }
                return { INVALID_OFFSET, size };
            }

            void Buffer::update(const Buffer::Space& space, const void* data) {
                glCall(glBufferSubData(m_type, space.offset * m_entrySize, space.size * m_entrySize, data));
            }

            void Buffer::remove(const Buffer::Space& space) {
                m_freeSpaces.push_back(space);
            }
            
            void Buffer::bind() const {
                glCall(glBindBuffer(m_type, m_renderingId));
            }
            void Buffer::unbind() const {
                glCall(glBindBuffer(m_type, 0));
            }

            GLsizeiptr Buffer::getFreeSpace() const {
                GLsizeiptr freeSpace = m_size - m_nextOffset;
                for (const auto& entry : m_freeSpaces) {
                    freeSpace += entry.size;
                }
                return freeSpace;
            }

        }
    }
}
