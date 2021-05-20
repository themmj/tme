#ifndef _CORE_GRAPHICS_SHADER_H
#define _CORE_GRAPHICS_SHADER_H
/** @file */

#include "core/graphics/common.hpp"
#include "core/loggable.hpp"
#include "core/storage.hpp"
#include <unordered_map>

namespace tme {
    namespace core {
        namespace graphics {

            /**//**
             * Abstraction for an OpenGL program object.
             *
             * Is constructed from two Shader::Stage handles when both stages are valid together.
             * Allows to set uniforms to pass extra data to the shader.
             */
            class Shader final : public Loggable, public Bindable {
                public:
                /**//**
                 * Abstraction for an OpenGL shader object.
                 *
                 * Is constructed from the contents of a file when the code does not contain any errors.
                 */
                class Stage final : public Loggable, public Mappable {
                    public:
                    /// available stage types
                    enum Type : uint32_t {
                        Vertex = GL_VERTEX_SHADER,
                        Fragment = GL_FRAGMENT_SHADER
                    };
                    /**//**
                     * Construct named shader stage from file.
                     *
                     * @param type the type of shader to be created
                     * @param name the name for the stage
                     * @param filePath path to the file containing the code for the stage
                     *
                     * @throw exceptions::InvalidInput when no file can be found for the provided filepath
                     * @throw exceptions::SyntaxError if the code in the file contains a syntax error
                     */
                    Stage(Type type, const std::string& name, const std::string& filePath);
                    ~Stage();

                    core::Identifier getId() const override;
                    /// get the stage type
                    inline Type getType() const { return m_type; }
                    /// get the path to the file used for creation
                    inline std::string getFilePath() const { return m_filePath; }
                    /// get the name provided during creation
                    inline std::string getName() const { return m_name; }

                    std::string toString() const override;

                    private:
                    Identifier m_id;
                    Type m_type;
                    std::string m_filePath;
                    std::string m_name;

                    void cleanUp();
                };

                private:
                core::Identifier m_stageCombination = 0;
                std::string m_name;
                std::unordered_map<std::string, GLint> m_uniformCache;

                public:
                /**//**
                 * Construct shader from two shader stages.
                 *
                 * Its name is the combination of the names of the provided stages.
                 *
                 * @param vertexStage handle to shader stage of type Stage::Type::Vertex
                 * @param fragmentStage handle to shader stage of type Stage::Typ::Fragment
                 *
                 * @throw exceptions::LinkingError when an error occurs during the linking process of the stages
                 * @throw exceptions::ValidationError when an error occurs during the validation of the shader object
                 */
                Shader(Handle<Shader::Stage> vertexStage, Handle<Shader::Stage> fragmentStage);
                ~Shader();

                /// returns the shaders identifier (shifted combination of the shader's stages)
                Identifier getId() const override;
                /// get name of the shader (combination of stage names)
                inline std::string getName() const { return m_name; }

                void bind() const override;
                void unbind() const override;

                /**//**
                 * Set one integer uniform.
                 *
                 * @param name name of the uniform
                 * @param value value the uniform should be set to
                 */
                void setUniform1i(const std::string& name, int value);
                /**//**
                 * Set four float uniform.
                 *
                 * @param name name of the uniform
                 * @param v0,v1,v2,v3 values the uniform should be set to
                 */
                void setUniform4f(const std::string& name, float  v0, float v1, float v2, float v3);

                std::string toString() const override;

                private:
                void cleanUp();
                GLint getUniformLocation(const std::string& name);
            };

        }
    }
}

#endif

