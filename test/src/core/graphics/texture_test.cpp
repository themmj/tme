#include "core/graphics/base.hpp"

#include "core/graphics/texture.hpp"
#include "core/exceptions/input.hpp"

namespace tme {
    namespace core {
        namespace graphics {

            TEST_F(GraphicsTest, LoadTexture) {
                const char* path = "../test/res/example.png";
                ASSERT_NO_THROW(Texture tex(path));
                Texture tex(path);
                EXPECT_EQ(tex.getWidth(), 700);
                EXPECT_EQ(tex.getHeight(), 700);
                EXPECT_STREQ(tex.getFilePath().c_str(), path);
                tex.bind();
                GLint slot;
                glGetIntegerv(GL_ACTIVE_TEXTURE, &slot);
                EXPECT_EQ(slot, GL_TEXTURE0 + tex.getSlot());
                tex.unbind();
            }

            TEST_F(GraphicsTest, LoadInvalidTexture) {
                const char* path = "../test/res/doesnotexist.png";
                ASSERT_THROW(Texture tex(path), exceptions::InvalidInput);
            }

        }
    }
}

