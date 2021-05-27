#include "core/graphics/base.hpp"

#include "core/graphics/vertex.hpp"

namespace tme {
    namespace core {
        namespace graphics {

            TEST_F(GraphicsTest, VertexLayoutCreation) {
                VertexLayout layout;
                layout.push<float>(2);
                layout.push<uint32_t>(4);
                layout.push<unsigned char>(6);

                auto elements = layout.getElements();
                ASSERT_EQ(elements.size(), 3);

                EXPECT_EQ(elements[0].type, GL_FLOAT);
                EXPECT_EQ(elements[0].typeSize, 4);
                EXPECT_EQ(elements[0].count, 2);
                EXPECT_EQ(elements[0].normalized, GL_FALSE);

                EXPECT_EQ(elements[1].type, GL_UNSIGNED_INT);
                EXPECT_EQ(elements[1].typeSize, 4);
                EXPECT_EQ(elements[1].count, 4);
                EXPECT_EQ(elements[1].normalized, GL_FALSE);

                EXPECT_EQ(elements[2].type, GL_UNSIGNED_BYTE);
                EXPECT_EQ(elements[2].typeSize, 1);
                EXPECT_EQ(elements[2].count, 6);
                EXPECT_EQ(elements[2].normalized, GL_TRUE);

                EXPECT_EQ(layout.getStride(), 2 * 4 + 4 * 4 + 6 * 1);
            }

            TEST_F(GraphicsTest, InvalidVertexLayoutPush) {
                int assertCountBefore = SignalCounter::instance()->get(SignalCounter::assertionFailed);
                VertexLayout layout;
                layout.push<long long>(1);
                Storage<_TestWindowApp>::global()->clear();
                EXPECT_EQ(SignalCounter::instance()->get(SignalCounter::assertionFailed), assertCountBefore + 1);
            }

            TEST_F(GraphicsTest, CreateVertexBuffer) {
                VertexBuffer vertex(8, 4);
                GLint boundBuffer;
                glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &boundBuffer);
                EXPECT_EQ(boundBuffer, vertex.getId());
            }

            TEST_F(GraphicsTest, CreateVertexArray) {
                auto vertex = Storage<VertexBuffer>::global()->create(12, 4);
                auto layout = Storage<VertexLayout>::global()->create();
                layout->push<float>(2);
                layout->push<unsigned char>(4);
                VertexArray array(vertex, layout);
                array.bind();

                GLint boundBuffer;
                glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &boundBuffer);
                EXPECT_EQ(boundBuffer, array.getVertexBuffer()->getId());

                auto elements = layout->getElements();
                for (GLuint i = 0; i < elements.size(); ++i) {
                     GLint bound;
                     GLint enabled;
                     GLsizei stride;
                     GLenum type;
                     GLint normalized;
                     glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &bound);
                     glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
                     glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &stride);
                     glGetVertexAttribIuiv(i, GL_VERTEX_ATTRIB_ARRAY_TYPE, &type);
                     glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_NORMALIZED, &normalized);
                     EXPECT_EQ(bound, array.getId());
                     EXPECT_NE(enabled, 0);
                     EXPECT_EQ(stride, layout->getStride());
                     EXPECT_EQ(type, elements[i].type);
                     EXPECT_EQ(normalized, elements[i].normalized);
                }
                array.unbind();
                Storage<VertexBuffer>::global()->clear();
                Storage<VertexLayout>::global()->clear();
            }

        }
    }
}

