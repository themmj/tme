#include "core/graphics/base.hpp"

#include "core/graphics/shader.hpp"
#include "core/exceptions/input.hpp"
#include "core/exceptions/validation.hpp"

namespace tme {
    namespace core {
        namespace graphics {

            TEST_F(GraphicsTest, LoadShaderStage) {
                const char* path = "../test/res/vertex-good.glsl";
                const char* name = "testname";
                ASSERT_NO_THROW(Shader::Stage stage(Shader::Stage::Type::Vertex, name, path));
                Shader::Stage stage(Shader::Stage::Type::Vertex, name, path);
                EXPECT_STREQ(stage.getFilePath().c_str(), path);
                EXPECT_STREQ(stage.getName().c_str(), name);
            }

            TEST_F(GraphicsTest, LoadInvalidShaderStage) {
                const char* path = "../test/res/vertex-not-there.glsl";
                const char* name = "testname";
                ASSERT_THROW(Shader::Stage stage(Shader::Stage::Type::Vertex, name, path), exceptions::InvalidInput);
            }

            TEST_F(GraphicsTest, LoadShaderStageWithSyntaxErrors) {
                const char* path = "../test/res/vertex-bad.glsl";
                const char* name = "testname";
                ASSERT_THROW(Shader::Stage stage(Shader::Stage::Type::Vertex, name, path), exceptions::SyntaxError);
            }

            TEST_F(GraphicsTest, CreateValidShader) {
                const char* vertexPath = "../test/res/vertex-good.glsl";
                const char* fragmentPath = "../test/res/fragment-good.glsl";
                ASSERT_NO_THROW(Shader::Stage stage(Shader::Stage::Type::Vertex, "vertex", vertexPath));
                ASSERT_NO_THROW(Shader::Stage stage(Shader::Stage::Type::Fragment, "fragment", fragmentPath));
                auto vertex = Storage<Shader::Stage>::global()->create(Shader::Stage::Type::Vertex, "vertex", vertexPath);
                auto fragment = Storage<Shader::Stage>::global()->create(Shader::Stage::Type::Fragment, "fragment", fragmentPath);
                ASSERT_NO_THROW(Shader shader(vertex, fragment));
                auto shader = Storage<Shader>::global()->create(vertex, fragment);
                EXPECT_STREQ(shader->getName().c_str(), "vertex|fragment");
                shader->bind();
                shader->unbind();
                Storage<Shader>::global()->clear();
                Storage<Shader::Stage>::global()->clear();
            }

        }
    }
}

