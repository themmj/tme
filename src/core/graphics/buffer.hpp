#ifndef _CORE_GRAPHICS_BUFFER_H
#define _CORE_GRAPHICS_BUFFER_H
/** @file */

#include <vector>
#include "core/graphics/common.hpp"
#include "core/loggable.hpp"
#include "core/storage.hpp"

namespace tme {
    namespace core {
        namespace graphics {

            /// invalid offset value one can check against
            constexpr GLsizeiptr INVALID_OFFSET = -1;

            /**//**
             * Abstraction of an OpenGL buffer.
             *
             * Provides infrastructure to create/delete an OpenGL buffer
             * and add/remove data. The removal of data does not clear that part
             * of the buffer and instead only allows to overwrite it in future
             * additions because the buffer can not know what is considered
             * "neutral" data.
             *
             * A Buffer is designed to store entries with a set size.
             * Because of that semantically the size and offset should be interpreted as entries
             * and not bytes. 
             */
            class Buffer : public Loggable, public Bindable {
                public:
                /// entity to store a destinct space inside a buffer
                struct Space {
                    /// offset inside the buffer
                    GLsizeiptr offset;
                    /// size of space
                    GLsizeiptr size;
                };

                private:
                GLenum m_type;
                GLsizeiptr m_entrySize;
                GLsizeiptr m_size;
                GLsizeiptr m_nextOffset;
                std::vector<Space> m_freeSpaces;

                public:
                /**//**
                 * Construct instance of a buffer of specific OpenGL buffer type
                 * with set size, allowing to store entries with a defined size.
                 *
                 * @param type OpenGL enum value for buffer type
                 * @param entrySize size in bytes of the individual entries
                 * @param size the amount of entries the buffer should have space for
                 */
                Buffer(GLenum type, GLsizeiptr entrySize, GLsizeiptr size);
                virtual ~Buffer();

                /**//**
                 * Try to add size amount of data somewhere into the buffer.
                 *
                 * Attempts to find space for the data. If not enough space is left inside the buffer
                 * the returned Space will contain an INVALID_OFFSET. In contrast to an exception this
                 * requires explicit success checks. Exceptions would increase the complexity of this low level
                 * abstraction and most likely overcomplicate orchestration logic.
                 *
                 * @param size amount of entries to be added to the buffer
                 * @param data pointer to entries
                 *
                 * @return Space containing storage information of the added data.
                 * Space.offset contains INVALID_OFFSET if not enough space is left in the buffer.
                 */
                Space add(GLsizeiptr size, const void* data);
                /**//**
                 * Release space inside the buffer to be overwritten.
                 *
                 * Does not clear the data as it cannot know "neutral" data.
                 *
                 * @param space the space to be released
                 */
                void remove(const Space& space);
                
                void bind() const override;
                void unbind() const override;

                /// get number of entries the buffer was instructed to be able to hold
                inline GLsizeiptr getSize() const { return m_size; }
                /// get number of entries still available inside the buffer
                GLsizeiptr getFreeSpace() const;
            };

        }
    }
}

#endif

