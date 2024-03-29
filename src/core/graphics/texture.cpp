/** @file */
#include "core/graphics/texture.hpp"
#include <sstream>
#include "stb/stb_image.h"
#include "core/exceptions/input.hpp"

namespace tme {
    namespace  core {
        namespace graphics {

            Texture::SlotGenerator Texture::s_slotGenerator = Texture::SlotGenerator();

            Texture::SlotGenerator::SlotGenerator() : m_nextSlot(0), m_freeSlots() {}
            Texture::SlotGenerator::~SlotGenerator() {}

            GLenum Texture::SlotGenerator::get() {
                if (m_freeSlots.size() > 0) {
                    GLenum back = m_freeSlots.back();
                    m_freeSlots.pop_back();
                    return back;
                }
                return m_nextSlot++;
            }

            void Texture::SlotGenerator::release(GLenum slot) {
                m_freeSlots.push_back(slot);
            }

            Texture::Texture(const std::string& filePath)
                : m_slot(s_slotGenerator.get()),
                m_filePath(filePath),
                m_width(0),
                m_height(0),
                m_channels(0) {
                stbi_set_flip_vertically_on_load(1);
                unsigned char* localBuffer = stbi_load(m_filePath.c_str(), &m_width, &m_height, &m_channels, 4);
                if (!localBuffer) {
                    TME_ERROR("could not read image file {}", m_filePath);
                    throw exceptions::InvalidInput("could not read image file");
                }

                glCall(glGenTextures(1, &m_renderingId));
                bind();

                glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
                glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
                glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
                glCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
                
                glCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA4, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, localBuffer));

                stbi_image_free(localBuffer);
                TME_INFO("created {}", *this);
            }

            Texture::~Texture() {
                TME_INFO("deleting {}", *this);
                unbind();
                glCall(glDeleteTextures(1, &m_renderingId));
                s_slotGenerator.release(m_slot);
            }

            void Texture::bind() const {
                glCall(glActiveTexture(GL_TEXTURE0 + m_slot));
                glCall(glBindTexture(GL_TEXTURE_2D, m_renderingId));
            }

            void Texture::unbind() const {
                glCall(glBindTexture(GL_TEXTURE_2D, 0));
            }

            std::string Texture::toString() const {
                std::stringstream ss;
                ss << "Texture(" << m_slot << ',' << m_width << ',' << m_height << ',' << m_filePath << ')';
                return ss.str();
            }

        }
    }
}

