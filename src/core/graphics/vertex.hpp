#ifndef _CORE_GRAPHICS_VERTEX_H
#define _CORE_GRAPHICS_VERTEX_H
/** @file */

#include <sstream>
#include <vector>
#include "core/graphics/common.hpp"
#include "core/graphics/buffer.hpp"
#include "core/loggable.hpp"
#include "core/storage.hpp"

namespace tme {
    namespace core {
        namespace graphics {

            /**//**
             * Store layout definition for vertex data.
             *
             * Allows to incrementally build up layout.
             */
            class VertexLayout final : public Loggable, public Mappable {
                public:
                /// entity storing info about a part of the vertex structure
                struct Element {
                    /// OpenGL type of the primitive
                    GLenum type;
                    /// size of primitive in bytes
                    GLsizei typeSize;
                    /// count of primitives
                    GLint count;
                    /// GL_TRUE if normalized GL_FALSE otherwise
                    GLboolean normalized;
                };

                private:
                Identifier m_id;
                std::vector<Element> m_elements;
                GLsizei m_stride;

                public:
                /// construct VertexLayout instance
                VertexLayout();
                ~VertexLayout();

                /**//**
                 * Adds Element based on provided type and count to the layout.
                 *
                 * @param count number of T contained inside element of vertex
                 */
                template<typename T>
                void push(GLint count) {
                    const Element element = getElement<T>(count);
                    m_elements.push_back(element);
                    m_stride += element.count * element.typeSize;
                }

                /// get stored elements defining the structure
                inline const std::vector<Element> getElements() const { return m_elements; }
                /// get stride of the vertex
                inline GLsizei getStride() const { return m_stride; }

                Identifier getId() const override { return m_id; }

                std::string toString() const override;

                private:
                template<typename T>
                const Element getElement(GLint) {
                    TME_ASSERT(false, "no available layout push function for provided type");
                    return {};
                }
            };

            /// Buffer implementation for a vertex buffer.
            class VertexBuffer final : public Buffer {
                public:
                /// construct Buffer instance for GL_ARRAY_BUFFER
                VertexBuffer(GLsizeiptr entrySize, GLsizeiptr size);
                ~VertexBuffer();

                std::string toString() const override;
            };

            /// combination of a vertex buffer and its corresponding layout
            class VertexArray final : public Loggable, public Bindable {
                Handle<VertexBuffer> m_vertexBuffer;
                Handle<VertexLayout> m_vertexLayout;
                public:
                /// construct VertexArray instance form VertexBuffer Handle and VertexLayout
                VertexArray(Handle<VertexBuffer> vertexBuffer, Handle<VertexLayout> vertexLayout);
                ~VertexArray();

                void bind() const override;
                void unbind() const override;

                /// get vertex buffer
                inline Handle<VertexBuffer> getVertexBuffer() const { return m_vertexBuffer; }
                /// get vertex layout
                inline Handle<VertexLayout> getVertexLayout() const { return m_vertexLayout; }

                std::string toString() const override;
            };

        }
    }
}

#endif

