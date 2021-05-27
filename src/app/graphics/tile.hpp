#ifndef _APP_GRAPHICS_TILE_H
#define _APP_GRAPHICS_TILE_H
/** @file */

#include "core/storage.hpp"
#include "core/graphics/batch.hpp"

namespace tme {
    namespace app {
        namespace graphics {

            /**//**
             * \brief Base class for tiles.
             *
             * All tiles should derive from this and provide a factory implementation to be usable with
             * the editor.
             * @sa TileFactory
             * Implements a default index, providing access to its definition, update functionality
             * and data access.
             */
            class Tile : public core::Loggable, public core::graphics::Batchable {
                protected:
                /**//**
                 * \brief Default index structure of a Tile.
                 *
                 * Consists of two triangles forming a square.
                 */
                struct Index {
                    /**//**
                     * \brief A single triangle.
                     */
                    struct Triangle {
                        /// first vertex index
                        unsigned int first;
                        /// first vertex index
                        unsigned int second;
                        /// first vertex index
                        unsigned int third;

                        /**//**
                         * \brief Construct Triangle from offset.
                         *
                         * For an offset of 3 the resulting Triangle contains 3, 4, 5.
                         *
                         * @param offset how much the 3 primitives should be offset
                         */
                        explicit Triangle(unsigned int offset)
                        : first(0 + offset),
                          second(1 + offset),
                          third(2 + offset) {}
                    };

                    /// first triangle
                    Triangle t1;
                    /// second triangle
                    Triangle t2;

                    /**//**
                     * \brief Construct Index from offset.
                     *
                     * As a square only uses four vertices the Triangles of the index will
                     * reuse two vertices.
                     * For an offset of 3 the resulting triangles would contain:
                     * 3, 4, 5 and
                     * 4, 5, 6.
                     *
                     * @param offset how much the initial vertex is offset from index 0
                     */
                    explicit Index(unsigned int offset): t1(offset), t2(offset + 1) {}
                };

                /// index value object
                Index m_indices;
                /// identifier of the tile
                core::Identifier m_id;
                /// x position of the tile in full tiles
                uint32_t m_x;
                /// y position of the tile in full tiles
                uint32_t m_y;
                /// global Identifier of the shader that should be used for the tile
                core::Identifier m_shaderId;
                /// allows to keep track of time in seconds, is updated by update()
                double m_clock;

                public:
                /**//**
                 * \brief Construct Tile instance.
                 *
                 * @param id identifier of the tile, should be provided by TileFactory
                 * @param x position of the tile on x axis in full tiles
                 * @param y position of the tile on y axis in full tiles
                 * @param shaderId global Identifier of the Shader to be used
                 */
                Tile(core::Identifier id, uint32_t x, uint32_t y, core::Identifier shaderId);
                virtual ~Tile();

                core::Identifier getId() const override;

                /**//**
                 * \brief Updates internal clock.
                 *
                 * Can be overloaded by derived classes to implement i.e. animations.
                 *
                 * @param deltaTime amount of time passed since the last update in seconds
                 *
                 * @return true if the Tile should be rerendered, false if nothing has changed
                 */
                virtual bool update(double deltaTime);

                virtual core::graphics::Batch::Config getBatchConfig() const override;
                virtual void setIndexOffset(unsigned int indexOffset) override;
                virtual const void* getIndexData() const override;
                virtual const void* getVertexData() const override;

                virtual std::string toString() const override;

                protected:
                /**//**
                 * \brief Provide Batch::Config::Index for default index structure.
                 *
                 * @return index description to be used in a batch config
                 */
                static core::graphics::Batch::Config::Index s_indexConfig();
            };


            /**//**
             * \brief Base factory class to create tiles.
             *
             * Factories for the other tile types should derive from this to be used inside the editor.
             * Stores position and shader storage.
             * Provides the generation for an tile id based on the tile position.
             */
            class TileFactory {
                protected:
                /// x position of the to be created tile
                uint32_t m_x;
                /// y position of the to be created tile
                uint32_t m_y;
                /// associated shader
                core::Identifier m_shaderId;

                public:
                /**//**
                 * \brief Construct TileFactory with shader.
                 *
                 * @param shaderId global id to the initial shader to be used for the tiles
                 */
                TileFactory(core::Identifier shaderId);
                virtual ~TileFactory();

                /**//**
                 * \brief Construct Tile from current state of the factory.
                 *
                 * @return owning pointer to newly created Tile
                 */
                virtual Tile* construct() = 0;

                /**//**
                 * \brief Generate id of a tile based on current state.
                 *
                 * @return id of the to be created Tile
                 */
                core::Identifier generateId() const;

                /**//**
                 * \brief Update position of the to be created Tile.
                 *
                 * @param x position in x direction in full tiles
                 * @param y position in y direction in full tiles
                 */
                void setPosition(uint32_t x, uint32_t y);

                /**//**
                 * \brief Update associated Shader.
                 *
                 * @param shaderId global id of the new Shader to be used for future tiles
                 */
                void setShader(core::Identifier shaderId);
                /**//**
                 * \brief Get global id of associated Shader.
                 *
                 * @return global id of the Shader used for future tiles
                 */
                inline core::Identifier getShader() const { return m_shaderId; }
            };

        }
    }
}

#endif

