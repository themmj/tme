#include "core/graphics/base.hpp"

#include "core/graphics/index.hpp"

namespace tme {
    namespace core {
        namespace graphics {

            TEST_F(GraphicsTest, CreateIndexBuffer) {
                IndexBuffer index(2, 8, 4);
                GLint boundBuffer;
                glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &boundBuffer);
                EXPECT_EQ(boundBuffer, index.getId());
                EXPECT_EQ(index.getPrimitiveCount(), 8);
            }

        }
    }
}

