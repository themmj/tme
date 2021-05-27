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
                int64_t bufferSize = entrySize * size;
                m_neutralBuffer = new unsigned char[(size_t)bufferSize];
                for (int64_t i = 0; i < bufferSize ; ++i) {
                    m_neutralBuffer[i] = 0;
                }
                glCall(glBufferData(type, bufferSize, m_neutralBuffer, GL_STATIC_DRAW));
            }
            Buffer::~Buffer() {
                delete[] m_neutralBuffer;
                unbind();
                glCall(glDeleteBuffers(1, &m_renderingId));
            }

            Buffer::Space Buffer::add(GLsizeiptr size, const void* data) {
                for (auto iter = m_freeSpaces.begin(); iter != m_freeSpaces.end(); ++iter) {
                    if (iter->size == size) {
                        Space dataSpace{ iter->offset, size };
                        update(dataSpace, data);
                        m_freeSpaces.erase(iter);
                        return dataSpace;
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
                bind();
                glCall(glBufferSubData(m_type, space.offset * m_entrySize, space.size * m_entrySize, data));
            }

            void Buffer::remove(const Buffer::Space& space) {
                if (space.offset != INVALID_OFFSET) {
                    update(space, m_neutralBuffer);
                    m_freeSpaces.push_back(space);
                }
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

            std::string Buffer::toString() const {
                std::stringstream ss;
                ss << "Buffer(" << getId() << ',';
                ss << m_type << ',';
                ss << m_entrySize << ',';
                ss << m_size << ')';
                return ss.str();
            }

        }
    }
}
