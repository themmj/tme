#ifndef _CORE_GRAPHICS_TEXTURE_H
#define _CORE_GRAPHICS_TEXTURE_H
/** @file */

#include "core/graphics/common.hpp"
#include "core/loggable.hpp"
#include <vector>

namespace tme {
    namespace core {
        namespace graphics {

            /// invalid texture identifier one can check against
            constexpr Identifier NO_TEXTURE = static_cast<Identifier>(-1);

            /**//**
             * \brief Abstraction to load an image from disk into an OpenGL texture.
             *
             * Can be set active on any available texture slot.
             */
            class Texture final : public Loggable, public Bindable {
                class SlotGenerator {
                    GLenum m_nextSlot;
                    std::vector<GLenum> m_freeSlots;
                    public:
                    SlotGenerator();
                    ~SlotGenerator();

                    GLenum get();
                    void release(GLenum slot);
                };
                static SlotGenerator s_slotGenerator;

                public:
                ///  type alias used for the dimensions of the image
                using Dimension = int;

                private:
                GLenum m_slot;
                std::string m_filePath;
                Dimension m_width, m_height;
                int m_channels;

                public:
                /**//**
                 * \brief Load image from filePath and create texture.
                 *
                 * @param filePath path to texture file
                 */
                Texture(const std::string& filePath);
                ~Texture();

                void bind() const override;
                void unbind() const override;

                /**//**
                 * \brief Get unique slot for texture to be active on.
                 *
                 * @return unique slot for the texture
                 */
                inline GLenum getSlot() const { return m_slot; }

                /**//**
                 * \brief Get width of the texture in pixels.
                 *
                 * @return width of the texture in pixels
                 */
                inline Dimension getWidth() const { return m_width; }
                /**//**
                 * \brief Get height of the texture in pixels.
                 *
                 * @return height of the texture in pixels
                 */
                inline Dimension getHeight() const { return m_height; }

                /**//**
                 * \brief Get file path.
                 *
                 * @return path to file on disk used for texture
                 */
                std::string getFilePath() const { return m_filePath; }

                std::string toString() const override;
            };

        }
    }
}

#endif

