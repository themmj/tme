#ifndef _CORE_GRAPHICS_BATCH_H
#define _CORE_GRAPHICS_BATCH_H
/** @file */

#include <string>
#include <unordered_map>
#include "core/graphics/common.hpp"
#include "core/graphics/texture.hpp"
#include "core/graphics/buffer.hpp"
#include "core/storage.hpp"
#include "core/graphics/vertex.hpp"
#include "core/graphics/index.hpp"

namespace tme {
    namespace core {
        namespace graphics {

            class Batchable;

            /**//**
             * \brief Storage and rendering abstraction to group similar graphics objects.
             *
             * A Batch allow to group graphic objects sharing the same vertex-, index structure,
             * shader and texture to be grouped together to reduce the amount of render calls made
             * to the GPU.
             * It is created with a set amount of graphics objects it should be able to store.
             */
            class Batch final : public Loggable, public Mappable, public Renderable {
                public:
                /**//**
                 * \brief Configuration of a batch.
                 *
                 * Contains description of the data that will be contained inside.
                 * Implements equality comparison operator for deep compare.
                 */
                struct Config final: public Loggable {
                    /**//**
                     * \brief Description of vertex data.
                     *
                     * Contains description of the vertex data that will be stored inside a batch.
                     * Implements equality comparison operator for deep compare.
                     */
                    struct Vertex : public Loggable {
                        /// amount of verticies of an object
                        size_t count;
                        /// size of individual vertex in bytes
                        size_t size;
                        /// global id of the layout of a vertex
                        Identifier layout;

                        /**//**
                         * \brief Construct vertex data definition.
                         *
                         * @param vertexCount number of verticies per graphics object
                         * @param vertexSize size of a single vertex in bytes
                         * @param vertexLayout id of the VertexLayout stored in global Storage
                         */
                        Vertex(size_t vertexCount, size_t vertexSize, Identifier vertexLayout);

                        /**//**
                         * \brief Deep equality operator.
                         *
                         * Compares itself with other by comparing count, size and layout.
                         *
                         * @param other right hand side value to compare itself against
                         *
                         * @return true if all member match, false otherwise
                         */
                        bool operator ==(const Vertex& other) const;

                        std::string toString() const override;
                    };
                    /**//**
                     * \brief Description of index data.
                     *
                     * Contains description of the index data that will be stored inside a batch.
                     * Implements equality comparison operator for deep compare.
                     */
                    struct Index final : public Loggable {
                        /// amount of indices of an object
                        size_t count;
                        /// size of individual index data structure in bytes
                        size_t size;
                        /// number of primitives inside individual index data structure
                        size_t primitiveCount;

                        /**//**
                         * \brief Construct index data definition.
                         *
                         * @param indexCount number of indices per graphics object
                         * @param indexSize size of a single index data structure in bytes
                         * @param indexPrimitiveCount number of primitives per index data structure
                         */
                        Index(size_t indexCount, size_t indexSize, size_t indexPrimitiveCount);

                        /**//**
                         * \brief Deep equality operator.
                         *
                         * Compares itself with other by comparing count, size and primitiveCount.
                         *
                         * @param other right hand side value to compare itself against
                         *
                         * @return true if all member match, false otherwise
                         */
                        bool operator ==(const Index& other) const;

                        std::string toString() const override;
                    };
                    /// vertex description
                    Vertex vertex;
                    /// index description
                    Index index;
                    /// global id of the shader used by the batch
                    Identifier shaderId;
                    /// global id of the texture used by the batch, NO_TEXTURE if it does not use a texture
                    Identifier textureId;
                    /// prerender hook to make custom shader and texture calls if necessary
                    void (*preRender)(Identifier, Identifier);

                    /**//**
                     * \brief Construct Config instance defaulting the textureId to NO_TEXTURE.
                     *
                     * The preRender is a function called before a batch is rendered. It has a
                     * Shader Identifier and a Texture Identifier as its parameters. In this hook one can
                     * bind the shader, add uniforms, bind the texture etc.
                     *
                     * @param vertexDefinition Config::Vertex describing the vertex data
                     * @param indexDefinition Config::Index describing the index data
                     * @param preRenderHook function called before rendering the batch
                     * @param shader Identifier for a Shader in global Storage
                     * @param texture Identifier for a Texture in global Storage or NO_TEXTURE by default
                     */
                    Config(const Vertex& vertexDefinition, const Index& indexDefinition, void (*preRenderHook)(Identifier, Identifier), Identifier shader, Identifier texture = NO_TEXTURE);

                    /**//**
                     * \brief Deep equality operator.
                     *
                     * Compares itself with other by comparing vertex, index, shaderId and textureId.
                     *
                     * @param other right hand side value to compare itself against
                     *
                     * @return true if all member match, false otherwise
                     */
                    bool operator ==(const Config& other) const;

                    std::string toString() const override;
                };
                /**//**
                 * \brief Value object defining an graphics object inside the batch.
                 *
                 * Contains the id of the batch and its corresponding Batch::Space for
                 * vertex and index data respectively.
                 */
                struct Entry {
                    /// id of the batch it is stored in
                    Identifier batchId;
                    /// buffer space of the vertex data
                    Buffer::Space vertexSpace;
                    /// buffer space of the index data
                    Buffer::Space indexSpace;
                };

                private:
                Identifier m_id;
                Handle<VertexBuffer> m_vertexBuffer;
                Handle<VertexArray> m_vertexArray;
                Handle<IndexBuffer> m_indexBuffer;
                Config m_config;

                public:
                /**//**
                 * \brief Construct Batch instance to hold objects similar to object.
                 *
                 * It will create the necessary buffer based on the size of the batch
                 * and its data definition from the batch config.
                 *
                 * @param size number of objects the batch should be able to hold
                 * @param config configuration of the batch
                 */
                Batch(size_t size, const Config& config);
                ~Batch();

                /**//**
                 * \brief Adds batchable object to the batch.
                 *
                 * Uses the setIndexOffset of the Batchable object to be able to insert the
                 * correct index data.
                 *
                 * @param object the object to be added to the batch
                 *
                 * @throw InsufficientBufferSpace when the Batch's buffer cannot store the requested data
                 *
                 * @return Entry instance describing the data inside the buffer to be used for updates or removal
                 */
                Entry add(Handle<Batchable> object);
                /**//**
                 * \brief Updates batchable object at entry in the batch.
                 *
                 * @param entry Entry describing the location of the data in the buffers
                 * @param object the Batchable with the new data
                 */
                void update(const Entry& entry, Handle<Batchable> object);
                /**//**
                 * \brief Remove data from batch.
                 *
                 * @param entry Entry describing the location of the data in the buffers
                 */
                void remove(const Entry& entry);

                void render() override;

                Identifier getId() const override { return m_id; }

                /**//**
                 * \brief Get the Batch's Config.
                 *
                 * @return configuration of the batch
                 */
                inline Config getConfig() const { return m_config; }

                std::string toString() const override;
            };

            /**//**
             * \brief Interface used to aggregate similar graphic objects in a batch.
             *
             * Types that want to use the buffer for graphics abstraction need to implement this
             * interface. It provides acces to the batch config for that type, accessors for its
             * vertex and index data as well as a way to set the offset of the indices.
             */
            class Batchable : public Mappable {
                public:
                /**//**
                 * \brief Get the batch configuration to be used for the object.
                 *
                 * @return Batch::Config describing the data
                 */
                virtual Batch::Config getBatchConfig() const = 0;

                /**//**
                 * \brief Get pointer to vertex data of the object.
                 *
                 * @return pointer to vertex data of the object
                 */
                virtual const void* getVertexData() const = 0;

                /**//**
                 * \brief Set index offset value.
                 *
                 * This should update the indices of the object. It is called by the batch after
                 * adding the vertex data because it only then knows what offset the index must be.
                 *
                 * @param indexOffset offset the indices should be set to
                 */
                virtual void setIndexOffset(unsigned int indexOffset) = 0;

                /**//**
                 * \brief Get pointer to index data of the object.
                 *
                 * It needs to return the index data based on the index offset provided by setIndexOffset.
                 *
                 * @return pointer to index data of the object
                 */
                virtual const void* getIndexData() const = 0;
            };

            /**//**
             * \brief Manager used to dynamically create needed Batch instances and add Batchable to them.
             *
             * Will create new batches when it does not have a suitable one for a Batchable object.
             * Otherwise the object will be added to an existing batch from which it can be removed as well.
             */
            class Batcher final : public Loggable, public Renderable {
                std::unordered_map<Identifier, Batch::Entry> m_mappings;
                Handle<Storage<Batch>> m_batches;
                size_t m_batchSize;

                public:
                /**//**
                 * \brief Create manager instance.
                 *
                 * @param batchSize the amount of objects all batches should be able to store
                 */
                Batcher(size_t batchSize);
                ~Batcher();

                /**//**
                 * \brief Insert if object not known, update otherwise.
                 *
                 * Will update the index of the Batchable object in the process.
                 * If either a Batch cannot be created or a Batchable object cannot be added to an Batch
                 * an error is logged as this should be caught in debug builds indicating an application issue.
                 *
                 * @param object the graphics object to be added
                 */
                void set(Handle<Batchable> object);
                /**//**
                 * \brief Remove object.
                 *
                 * Finds the corresponding batch and removes the data from its buffers.
                 *
                 * @param object the graphics object to be removed
                 */
                void unset(Handle<Batchable> object);

                void render() override;

                std::string toString() const override;
            };

        }
    }
}

#endif

