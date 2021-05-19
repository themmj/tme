#ifndef _CORE_GRAPHICS_TEXTURE_H
#define _CORE_GRAPHICS_TEXTURE_H
/** @file */

#include "core/graphics/common.hpp"
#include "core/loggable.hpp"

namespace tme {
    namespace core {
        namespace graphics {

            /**//**
             * Abstraction to load an image from disk into an OpenGL texture.
             *
             * Can be set active on any available texture slot.
             */
            class Texture final : public Loggable, public Bindable {
                public:
                ///  type alias used for the dimensions of the image
                using Dimension = int;

                private:
                std::string m_filePath;
                Dimension m_width, m_height;
                int m_channels;

                public:
                /// load image from filePath and create texture
                Texture(const std::string& filePath);
                ~Texture();

                void bind() const override;
                void unbind() const override;

                /**//**
                 * Set texture on slot active.
                 *
                 * @param slot index of the texture to be used in the shader, default 0
                 */
                void setActive(GLenum slot = 0) const;

                /// get width of the texture in pixels
                inline Dimension getWidth() const { return m_width; }
                /// get height of the texture in pixels
                inline Dimension getHeight() const { return m_height; }

                std::string toString() const override;
            };

        }
    }
}

#endif

