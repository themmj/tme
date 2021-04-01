/** @file */
#include "core/graphics/shader.hpp"
#include "core/map.hpp"
#include <fstream>
#include <iterator>
#include <sstream>

namespace tme {
    namespace  core {
        namespace graphics {

            Shader::Stage::Stage(Type type, const std::string& filePath) : m_id(0), m_type(type) {
                std::ifstream file(filePath);
                if (!file) {
                    TME_ERROR("could not find file {}", filePath);
                    return;
                }
                std::string content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
                file.close();

                glCall(GLuint id = glCreateShader(m_type));
                auto src = &content[0];
                glCall(glShaderSource(id, 1, &src, nullptr));

                glCall(glCompileShader(id));
                if (!logGLStatus(GL_STATUS_FNS(Shader), id, GL_COMPILE_STATUS)) {
                    TME_ERROR("could not compile {}", *this);
                    return;
                }
                TME_INFO("compiled {} from file {}", *this, filePath);
                m_id = id;
            }

            Shader::Stage::~Stage() {
                if (m_id) {
                    TME_INFO("deleting {}", *this);
                    glCall(glDeleteShader(m_id));
                }
            }

            core::Identifier Shader::Stage::getId() const {
                return m_id;
            }

            std::string Shader::Stage::toString() const {
                std::stringstream ss;
                ss << "Shader::Stage" << '(';
                if (m_type == Shader::Stage::Type::Vertex) {
                    ss << "vertex";
                } else if (m_type == Shader::Stage::Type::Fragment) {
                    ss << "fragment";
                } else {
                    ss << "unknown";
                }
                ss << ':' << m_id << ')';
                return ss.str();
            }


            Shader::Shader(Handle<Shader::Stage> vertexStage, Handle<Shader::Stage> fragmentStage)
                : m_uniformCache() {
                TME_ASSERT(vertexStage, "provided invalid vertex stage");
                TME_ASSERT(fragmentStage, "provided invalid fragment stage");

                m_stageCombination = static_cast<core::Identifier>(fragmentStage->m_id) | (static_cast<core::Identifier>(vertexStage->m_id) << 32);

                glCall(GLuint id = glCreateProgram());
                glCall(glAttachShader(id, vertexStage->m_id));
                glCall(glAttachShader(id, fragmentStage->m_id));

                glCall(glLinkProgram(id));
                if (!logGLStatus(GL_STATUS_FNS(Program), id, GL_LINK_STATUS)) {
                    TME_ERROR("could not link shader for stages {},{}", vertexStage, fragmentStage);
                    return;
                }

                glCall(glValidateProgram(id));
                if (!logGLStatus(GL_STATUS_FNS(Program), id, GL_VALIDATE_STATUS)) {
                    TME_ERROR("could not validate shader for stages {},{}", vertexStage, fragmentStage);
                    return;
                }

                TME_INFO("created {}", *this);
                m_renderingId = id;
            }

            Shader::~Shader() {
                TME_INFO("deleting {}", *this);
                unbind();
            }

             core::Identifier Shader::getId() const {
                return m_stageCombination;
             }

            void Shader::bind() const {
                glCall(glUseProgram(m_renderingId));
            }

            void Shader::unbind() const {
                glCall(glUseProgram(0));
            }

            void Shader::setUniform1i(const std::string& name, int value) {
                glCall(glUniform1i(getUniformLocation(name), value));
            }

            void Shader::setUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
                glCall(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
            }

            GLint Shader::getUniformLocation(const std::string& name) {
                if(m_uniformCache.find(name) != m_uniformCache.end()) {
                    return m_uniformCache[name];
                }
                glCall(GLint location = glGetUniformLocation(m_renderingId, name.c_str()));
                if (location == -1) {
                    TME_WARN("unknown uniform '{}' for shader {}", name, m_renderingId);
                }
                m_uniformCache[name] = location;
                return location;
            }

            std::string Shader::toString() const {
                std::stringstream ss;
                ss << "Shader(" << m_renderingId << '|';
                ss << (m_stageCombination << 32 >> 32) << ',';
                ss << (m_stageCombination >> 32) << ')';
                return ss.str();
            }

        }
    }
}

