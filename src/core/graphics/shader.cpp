/** @file */
#include <fstream>
#include <iterator>
#include <sstream>
#include "core/graphics/shader.hpp"
#include "core/storage.hpp"
#include "core/exceptions/input.hpp"
#include "core/exceptions/validation.hpp"

namespace tme {
    namespace  core {
        namespace graphics {

            Shader::Stage::Stage(Type type, const std::string& name, const std::string& filePath) : m_type(type), m_filePath(filePath), m_name(name) {
                std::ifstream file(filePath);
                if (!file.is_open()) {
                    TME_ERROR("could not find file {}", filePath);
                    throw exceptions::InvalidInput("could not find file");
                }
                std::string content((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
                file.close();

                glCall(m_id = glCreateShader(m_type));
                auto contentCharPtr = &content[0];
                glCall(glShaderSource(m_id, 1, &contentCharPtr, nullptr));

                glCall(glCompileShader(m_id));
                if (!logGLStatus(GL_STATUS_FNS(Shader), m_id, GL_COMPILE_STATUS)) {
                    TME_ERROR("could not compile {}", *this);
                    cleanUp();
                    throw exceptions::SyntaxError("could not compile shader stage, see logs for details");
                }
                TME_INFO("created {}", *this);
            }

            Shader::Stage::~Stage() {
                TME_INFO("deleting {}", *this);
                cleanUp();
            }

            void Shader::Stage::cleanUp() {
                glCall(glDeleteShader(m_id));
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
                ss << ':' << m_id << ',' << m_name ;
                ss << ',' << m_filePath << ')';
                return ss.str();
            }


            Shader::Shader(Handle<Shader::Stage> vertexStage, Handle<Shader::Stage> fragmentStage)
                : m_uniformCache() {
                TME_ASSERT(vertexStage, "provided invalid vertex stage");
                TME_ASSERT(fragmentStage, "provided invalid fragment stage");

                m_stageCombination = fragmentStage->getId() | (vertexStage->getId() << 16);

                glCall(m_renderingId = glCreateProgram());
                glCall(glAttachShader(m_renderingId, vertexStage->getId()));
                glCall(glAttachShader(m_renderingId, fragmentStage->getId()));

                glCall(glLinkProgram(m_renderingId));
                if (!logGLStatus(GL_STATUS_FNS(Program), m_renderingId, GL_LINK_STATUS)) {
                    TME_ERROR("could not link shader for stages {},{}", vertexStage, fragmentStage);
                    cleanUp();
                    throw exceptions::LinkingError("could not link shader, see logs for details");
                }

                glCall(glValidateProgram(m_renderingId));
                if (!logGLStatus(GL_STATUS_FNS(Program), m_renderingId, GL_VALIDATE_STATUS)) {
                    TME_ERROR("could not validate shader for stages {},{}", vertexStage, fragmentStage);
                    cleanUp();
                    throw exceptions::ValidationError("could not validate shader, see logs for details");
                }

                TME_INFO("created {}", *this);
            }

            Shader::~Shader() {
                TME_INFO("deleting {}", *this);
                cleanUp();
            }

            void Shader::cleanUp() {
                unbind();
                glCall(glDeleteProgram(m_renderingId));
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

            void Shader::setUniformMat4f(const std::string& name, const glm::mat4& matrix) {
                glCall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
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
                ss << (m_stageCombination << 16 >> 16) << ',';
                ss << (m_stageCombination >> 16) << ')';
                return ss.str();
            }

        }
    }
}

