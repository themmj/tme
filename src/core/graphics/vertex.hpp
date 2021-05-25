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
             * \brief Store layout definition for vertex data.
             *
             * Allows to incrementally build up layout.
             */
            class VertexLayout final : public Loggable, public Mappable {
                public:
                /**//**
                 * \brief Value object storing info about a part of the vertex structure.
                 *
                 * Contains necessary information to enable a vertex attribute pointer.
                 */
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
                /**//**
                 * \brief Construct VertexLayout instance.
                 */
                VertexLayout();
                ~VertexLayout();

                /**//**
                 * \brief Adds Element based on provided type and count to the layout.
                 *
                 * @param count number of T contained inside element of vertex
                 */
                template<typename T>
                void push(GLint count) {
                    const Element element = getElement<T>(count);
                    m_elements.push_back(element);
                    m_stride += element.count * element.typeSize;
                }

                /**//**
                 * \brief Get stored elements defining the structure.
                 *
                 * @return vector of description elements pushed into the layout
                 */
                inline const std::vector<Element> getElements() const { return m_elements; }
                /**//**
                 * \brief Get stride of the vertex.
                 *
                 * @return stride of the layout
                 */
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

            /**//**
             * \brief Buffer implementation for a vertex buffer.
             */
            class VertexBuffer final : public Buffer {
                public:
                /**//**
                 * \brief Construct Buffer instance.
                 *
                 * Uses GL_ARRAY_BUFFER for the type.
                 *
                 * @param entrySize size of a single vertex entry in bytes
                 * @param size amount of vertices the buffer should be able to store
                 */
                VertexBuffer(GLsizeiptr entrySize, GLsizeiptr size);
                ~VertexBuffer();

                std::string toString() const override;
            };

            /**//**
             * \brief Combination of a VertexBuffer and its corresponding VertexLayout.
             */
            class VertexArray final : public Loggable, public Bindable {
                Handle<VertexBuffer> m_vertexBuffer;
                Handle<VertexLayout> m_vertexLayout;
                public:
                /**//**
                 * \brief Construct VertexArray instance.
                 *
                 * @param vertexBuffer VertexBuffer to use
                 * @param vertexLayout VertexLayout to use
                 */
                VertexArray(Handle<VertexBuffer> vertexBuffer, Handle<VertexLayout> vertexLayout);
                ~VertexArray();

                void bind() const override;
                void unbind() const override;

                /**//**
                 * \brief Get vertex buffer.
                 *
                 * @return Handle to the used VertexBuffer
                 */
                inline Handle<VertexBuffer> getVertexBuffer() const { return m_vertexBuffer; }
                /**//**
                 * \brief Get vertex layout.
                 *
                 * @return Handle to the used VertexLayout
                 */
                inline Handle<VertexLayout> getVertexLayout() const { return m_vertexLayout; }

                std::string toString() const override;
            };

        }
    }
}

#endif

