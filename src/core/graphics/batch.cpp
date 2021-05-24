/** @file */
#include "core/graphics/batch.hpp"
#include <sstream>
#include "core/graphics/vertex.hpp"
#include "core/graphics/index.hpp"
#include "core/graphics/shader.hpp"
#include "core/graphics/texture.hpp"
#include "core/exceptions/input.hpp"

namespace tme {
    namespace  core {
        namespace graphics {

            Batch::Config::Vertex::Vertex(size_t vertexCount, size_t vertexSize, Identifier vertexLayout)
                : count(vertexCount), size(vertexSize), layout(vertexLayout) {}

            bool Batch::Config::Vertex::operator==(const Batch::Config::Vertex& other) const {
                return this->count == other.count && this->size == other.size && this->layout == other.layout;
            }

            std::string Batch::Config::Vertex::toString() const {
                std::stringstream ss;
                ss << "Vertex(" << count << ',' << size << ',' << layout << ')';
                return ss.str();
            }

            Batch::Config::Index::Index(size_t indexCount, size_t indexSize, size_t indexPrimitiveCount)
                : count(indexCount), size(indexSize), primitiveCount(indexPrimitiveCount) {}

            bool Batch::Config::Index::operator==(const Batch::Config::Index& other) const {
                return this->count == other.count && this->size == other.size && this->primitiveCount == other.primitiveCount;
            }
            std::string Batch::Config::Index::toString() const {
                std::stringstream ss;
                ss << "Index(" << count << ',' << size << ',' << primitiveCount << ')';
                return ss.str();
            }

            Batch::Config::Config(const Vertex& vertexDefinition, const Index& indexDefinition, void (*preRenderHook)(Identifier, Identifier), Identifier shader, Identifier texture)
                : vertex(vertexDefinition), index(indexDefinition), shaderId(shader), textureId(texture), preRender(preRenderHook) {}

            bool Batch::Config::operator==(const Batch::Config& other) const {
                return this->shaderId == other.shaderId && this->textureId == other.textureId && this->vertex == other.vertex && this->index == other.index;
            }
            std::string Batch::Config::toString() const {
                std::stringstream ss;
                ss << "Config(" << vertex << ',' << index << ',';
                ss << shaderId << ',' << textureId << ',' << preRender << ')';
                return ss.str();
            }

            Batch::Batch(size_t size, const Batch::Config& config)
                : m_id(uuid<Batch>()), m_config(config) {
                auto layout = Storage<VertexLayout>::global()->get(m_config.vertex.layout);
                if (!layout) {
                    throw exceptions::InvalidInput("could not find vertex layout with provided id");
                }
                m_vertexBuffer = Storage<VertexBuffer>::global()->create(config.vertex.size, config.vertex.count * size);
                m_vertexArray = Storage<VertexArray>::global()->create(m_vertexBuffer, layout);
                m_indexBuffer = Storage<IndexBuffer>::global()->create(config.index.primitiveCount, config.index.size, config.index.count * size);
                TME_INFO("created {}", *this);
            }

            Batch::~Batch() {
                TME_INFO("deleting {}", *this);
                Storage<VertexBuffer>::global()->destroy(m_vertexArray->getVertexBuffer()->getId());
                Storage<VertexArray>::global()->destroy(m_vertexArray->getId());
                Storage<IndexBuffer>::global()->destroy(m_indexBuffer->getId());
            }

            Batch::Entry Batch::add(Handle<Batchable> object) {
                TME_ASSERT(object->getBatchConfig() == m_config, "trying to add unsuitable data to batch");
                Buffer::Space vertexSpace = m_vertexBuffer->add(static_cast<GLsizeiptr>(m_config.vertex.count), object->getVertexData());
                // losing larger values is ok (if they exceed 32 bit something is really off in the data definition)
                object->setIndexOffset((unsigned int)vertexSpace.offset);
                Buffer::Space indexSpace = m_indexBuffer->add(static_cast<GLsizeiptr>(m_config.index.count), object->getIndexData());
                Batch::Entry e;
                e.batchId = getId();
                e.vertexSpace = vertexSpace;
                e.indexSpace = indexSpace;
                return e;
            }

            void Batch::update(const Entry& entry, Handle<Batchable> object) {
                TME_ASSERT(object->getBatchConfig() == m_config, "trying to add unsuitable data to batch");
                m_vertexBuffer->update(entry.vertexSpace, object->getVertexData());
                m_indexBuffer->update(entry.indexSpace, object->getIndexData());
            }

            void Batch::remove(const Entry& entry) {
                m_vertexBuffer->remove(entry.vertexSpace);
                m_indexBuffer->remove(entry.indexSpace);
            }

            void Batch::render() {
                m_vertexArray->bind();
                m_indexBuffer->bind();
                m_config.preRender(m_config.shaderId, m_config.textureId);
                glCall(glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indexBuffer->getPrimitiveCount()), GL_UNSIGNED_INT, nullptr));
            }

            std::string Batch::toString() const {
                std::stringstream ss;
                ss << "Batch(" << m_id << ',';
                ss << *m_vertexArray << ',';
                ss << *m_indexBuffer << ',';
                ss << m_config << ')';
                return ss.str();
            }

            Batcher::Batcher(size_t batchSize) : m_mappings(), m_batches(Storage<Batch>::localInstance()), m_batchSize(batchSize) {
                TME_INFO("created {}", *this);
            }

            Batcher::~Batcher() {
                TME_INFO("deleting {}", *this);
                m_mappings.clear();
                m_batches->clear();
            }

            void Batcher::set(Handle<Batchable> object) {
                if (const auto& iter = m_mappings.find(object->getId()); iter != m_mappings.end()) {
                    if (auto previousBatch = m_batches->get(iter->second.batchId); previousBatch) {
                        if (previousBatch->getConfig() == object->getBatchConfig()) {
                            previousBatch->update(iter->second, object);
                            return;
                        } else {
                            previousBatch->remove(iter->second);
                        }
                    }
                }
                for (const auto& iter : *m_batches) {
                    if (iter.second->getConfig() == object->getBatchConfig()) {
                        Batch::Entry newEntry = iter.second->add(object);
                        m_mappings.insert_or_assign(object->getId(), newEntry);
                        return;
                    }
                }
                try {
                    auto newBatch = m_batches->create(m_batchSize, object->getBatchConfig());
                    Batch::Entry newEntry = newBatch->add(object);
                    m_mappings.insert_or_assign(object->getId(), newEntry);
                } catch (const exceptions::InvalidInput& e) {
                    TME_ERROR("could not create new batch {}, {}", e.type(), e.what());
                }
            }

            void Batcher::unset(Handle<Batchable> object) {
                if (const auto& iter = m_mappings.find(object->getId()); iter != m_mappings.end()) {
                    if (auto batch = m_batches->get(iter->second.batchId); batch) {
                        batch->remove(iter->second);
                        m_mappings.erase(object->getId());
                    }
                }
            }

            void Batcher::render() {
                for (const auto& iter : *m_batches) {
                    iter.second->render();
                }
            }

            std::string Batcher::toString() const {
                std::stringstream ss;
                ss << "Batcher(";
                for (const auto& iter : *m_batches) {
                    ss << *iter.second;
                }
                for (const auto& iter : m_mappings) {
                    ss << "Map(" << iter.first << ',' << iter.second.batchId << ')';
                }
                ss << ')';
                return ss.str();
            }

        }
    }
}

