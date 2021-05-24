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

            /// storage and rendering abstraction to group similar graphics objects
            class Batch final : public Loggable, public Mappable, public Renderable {
                public:
                /// configuration of a batch
                struct Config final: public Loggable {
                    /// configuration of the vertex data of an object
                    struct Vertex : public Loggable {
                        /// amount of verticies of an object
                        size_t count;
                        /// size of individual vertex in bytes
                        size_t size;
                        /// global id of the layout of a vertex
                        Identifier layout;

                        /// construct vertex data definition
                        Vertex(size_t vertexCount, size_t vertexSize, Identifier vertexLayout);

                        /// equality operator
                        bool operator ==(const Vertex& other) const;

                        std::string toString() const override;
                    };
                    /// configuration of the index data of an object
                    struct Index final : public Loggable {
                        /// amount of indices of an object
                        size_t count;
                        /// size of individual index data structure in bytes
                        size_t size;
                        /// number of primitives inside individual index data structure
                        size_t primitiveCount;

                        /// construct vertex data definition
                        Index(size_t indexCount, size_t indexSize, size_t indexPrimitiveCount);

                        /// equality operator
                        bool operator ==(const Index& other) const;

                        std::string toString() const override;
                    };
                    /// vertex configuration
                    Vertex vertex;
                    /// index configuration
                    Index index;
                    /// global id of the shader used by the batch
                    Identifier shaderId;
                    /// global id of the texture used by the batch, NO_TEXTURE if it does not use a texture
                    Identifier textureId;
                    /// prerender hook to make custom shader and texture calls if necessary
                    void (*preRender)(Identifier, Identifier);

                    /// construct Config instance defaulting the textureId to NO_TEXTURE
                    Config(const Vertex& vertexDefinition, const Index& indexDefinition, void (*preRenderHook)(Identifier, Identifier), Identifier shader, Identifier texture = NO_TEXTURE);

                    /// equality operator
                    bool operator ==(const Config& other) const;

                    std::string toString() const override;
                };
                /// value object defining an graphics object inside the batch
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
                 * Construct Batch instance to hold objects similar to object.
                 *
                 * @param size number of objects the batch should be able to hold
                 * @param config configuration of the batch
                 */
                Batch(size_t size, const Config& config);
                ~Batch();

                /// adds batchable object to the batch, sets index offset of the object
                Entry add(Handle<Batchable> object);
                /// updates batchable object at entry in the batch
                void update(const Entry& entry, Handle<Batchable> object);
                /// remove data from batch
                void remove(const Entry& entry);

                void render() override;

                Identifier getId() const override { return m_id; }

                /// get the batche's configuration
                inline Config getConfig() const { return m_config; }

                std::string toString() const override;
            };

            /// interface used to aggregate similar graphic objects in a batch
            class Batchable : public Mappable {
                public:
                /// get the batch configuration to be used for the object
                virtual Batch::Config getBatchConfig() const = 0;

                /// get pointer to vertex data of the object
                virtual const void* getVertexData() const = 0;

                /// set index offset value, needed to be dynamic
                virtual void setIndexOffset(unsigned int indexOffset) = 0;

                /// get pointer to index data of the object, needs to consider index offset
                virtual const void* getIndexData() const = 0;
            };

            /// manager used to dynamically create needed batches and add graphics objects to them
            class Batcher final : public Loggable, public Renderable {
                std::unordered_map<Identifier, Batch::Entry> m_mappings;
                Handle<Storage<Batch>> m_batches;
                size_t m_batchSize;

                public:
                /// create manager instance
                Batcher(size_t batchSize);
                ~Batcher();

                /// insert if object not known, update otherwise
                /// updates index offset if needed
                void set(Handle<Batchable> object);
                /// remove object
                void unset(Handle<Batchable> object);

                void render() override;

                std::string toString() const override;
            };

        }
    }
}

#endif

