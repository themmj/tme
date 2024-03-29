/** @file */
#include "core/graphics/vertex.hpp"
#include <sstream>
#include "core/storage.hpp"

namespace tme {
    namespace  core {
        namespace graphics {

            VertexLayout::VertexLayout() : m_id(uuid<VertexLayout>()), m_elements(), m_stride(0) {
                TME_INFO("created {}", *this);
            }

            VertexLayout::~VertexLayout() {
                TME_INFO("deleting {}", *this);
            }

            std::string VertexLayout::toString() const {
                std::stringstream ss;
                ss << "VertexLayout:" << m_id << '[';
                for (const auto& element : m_elements) {
                    ss << '(' << element.type << ',';
                    ss << element.typeSize << ',';
                    ss << element.count << ',';
                    if (element.normalized == GL_TRUE) {
                        ss << 1;
                    } else {
                        ss << 0;
                    }
                    ss << ')';
                }
                ss << ']';
                return ss.str();
            }

            template<>
            const VertexLayout::Element VertexLayout::getElement<float>(GLint count) {
                return {GL_FLOAT, 4, count, GL_FALSE};
            }

            template<>
            const VertexLayout::Element VertexLayout::getElement<uint32_t>(GLint count) {
                return {GL_UNSIGNED_INT, 4, count, GL_FALSE};
            }

            template<>
            const VertexLayout::Element VertexLayout::getElement<unsigned char>(GLint count) {
                return {GL_UNSIGNED_BYTE, 1, count, GL_TRUE};
            }


            VertexBuffer::VertexBuffer(GLsizeiptr entrySize, GLsizeiptr size) : Buffer(GL_ARRAY_BUFFER, entrySize, size) {
                TME_INFO("created {}", *this);
            }
            VertexBuffer::~VertexBuffer() {
                TME_INFO("deleting {}", *this);
            }

            std::string VertexBuffer::toString() const {
                std::stringstream ss;
                ss << "VertexBuffer(" << getId() << ',' << getSize() << ',' << getFreeSpace() << ')';
                return ss.str();
            }


            VertexArray::VertexArray(Handle<VertexBuffer> vertexBuffer, Handle<VertexLayout> vertexLayout)
            : m_vertexBuffer(vertexBuffer), m_vertexLayout(vertexLayout) {
                glCall(glGenVertexArrays(1, &m_renderingId));
                bind();
                vertexBuffer->bind();
                int64_t offset = 0;
                const auto& elements = vertexLayout->getElements();
                for (GLuint i = 0; i < elements.size(); ++i) {
                    const auto& element = elements[i];
                    glCall(glEnableVertexAttribArray(i));
                    glCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, vertexLayout->getStride(), (const GLvoid*) offset));
                    offset += element.count * element.typeSize;
                }
                TME_INFO("created {}", *this);
            }

            VertexArray::~VertexArray() {
                TME_INFO("deleting {}", *this);
                unbind();
                glCall(glDeleteVertexArrays(1, &m_renderingId));
            }

            void VertexArray::bind() const {
                glCall(glBindVertexArray(m_renderingId));
            }
            void VertexArray::unbind() const {
                glCall(glBindVertexArray(0));
            }

            std::string VertexArray::toString() const {
                std::stringstream ss;
                ss << "VertexArray(" << getId() << ',';
                ss << m_vertexBuffer->toString() << ',';
                ss << m_vertexLayout->toString() << ')';
                return ss.str();
            }

        }
    }
}
