#ifndef _CORE_GRAPHICS_SHADER_H
#define _CORE_GRAPHICS_SHADER_H
/** @file */

#include "core/graphics/common.hpp"
#include "core/loggable.hpp"
#include "core/map.hpp"
#include <unordered_map>

namespace tme {
    namespace core {
        namespace graphics {

            class Shader final : public Loggable, Bindable, core::Mappable {
                public:
                class Stage final : public Loggable, Mappable {
                    friend Shader;

                    public:
                    enum Type : uint32_t {
                        Vertex = GL_VERTEX_SHADER,
                        Fragment = GL_FRAGMENT_SHADER
                    };

                    Stage(Type type, const std::string& filePath);
                    ~Stage();

                    core::Identifier getId() const override;
                    inline Type getType() const { return m_type; }

                    std::string toString() const override;

                    private:
                    RenderingId m_id;
                    Type m_type;
                };

                private:
                core::Identifier m_stageCombination = 0;
                std::unordered_map<std::string, GLint> m_uniformCache;

                public:
                Shader(Handle<Shader::Stage> vertexStage, Handle<Shader::Stage> fragmentStage);
                ~Shader();

                core::Identifier getId() const override;

                void bind() const override;
                void unbind() const override;

                void setUniform1i(const std::string& name, int value);
                void setUniform4f(const std::string& name, float  v0, float v1, float v2, float v3);

                std::string toString() const override;

                private:
                GLint getUniformLocation(const std::string& name);
            };

        }
    }
}

#endif

