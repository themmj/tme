#include "core/exceptions/graphics.hpp"
#include "core/graphics/base.hpp"

#include "core/graphics/batch.hpp"
#include "core/exceptions/input.hpp"

namespace tme {
    namespace core {
        namespace graphics {

            class _ExampleData final : public Batchable {
                public:
                float vertices[16];
                unsigned int indices[6];
                Identifier id;
                unsigned int indexOffset;
                size_t sizeDiff;

                static uint32_t preRenderHookCount;

                _ExampleData(size_t indexSizeDiff = 0) : id(uuid<_ExampleData>()), sizeDiff(indexSizeDiff) {
                    
                }

                Batch::Config getBatchConfig() const override {
                    auto layouts = Storage<VertexLayout>::global();
                    auto layout = layouts->create();
                    layout->push<float>(2);
                    layout->push<float>(2);

                    static Batch::Config::Vertex vertex(4, sizeof(float) * 4, layout->getId());
                    Batch::Config::Index index(1, sizeof(unsigned int) * (6 + sizeDiff), 6);
                    Batch::Config config(vertex, index, [](Identifier, Identifier){
                            ++preRenderHookCount;
                        }, 0);
                    return config;
                }

                const void* getVertexData() const override { return vertices; }
                const void* getIndexData() const override { return indices; }
                void setIndexOffset(unsigned int offset) override { indexOffset = offset; }
                Identifier getId() const override { return id; }
            };

            uint32_t _ExampleData::preRenderHookCount = 0;

            TEST_F(GraphicsTest, CreateValidBatch) {
                auto dataStore = Storage<_ExampleData>::localInstance();
                auto data = dataStore->create();
                auto config = data->getBatchConfig();
                ASSERT_NO_THROW(Batch b(2, config));
                Batch b(2, config);
                EXPECT_EQ(b.getConfig(), config);
            }

            TEST_F(GraphicsTest, CreateBatchWithoutLayout) {
                auto dataStore = Storage<_ExampleData>::localInstance();
                auto data = dataStore->create();
                auto config = data->getBatchConfig();
                config.vertex.layout = -1;
                ASSERT_THROW(Batch b(2, config), exceptions::InvalidInput);
            }

            TEST_F(GraphicsTest, AddValuesToBatch) {
                auto dataStore = Storage<_ExampleData>::localInstance();
                auto data1 = dataStore->create();
                auto data2 = dataStore->create();
                auto config = data1->getBatchConfig();
                ASSERT_NO_THROW(Batch b(2, config));
                Batch b(2, config);

                Batch::Entry de1;
                Batch::Entry de2;
                ASSERT_NO_THROW(de1 = b.add(data1));
                ASSERT_NO_THROW(de2 = b.add(data2));

                EXPECT_EQ(data1->indexOffset, 0);
                EXPECT_EQ(data2->indexOffset, 4);

                EXPECT_EQ(de1.batchId, b.getId());
                EXPECT_EQ(de2.batchId, b.getId());

                EXPECT_EQ(de1.vertexSpace.offset, 0);
                EXPECT_EQ(de1.vertexSpace.size, 4);
                EXPECT_EQ(de2.vertexSpace.offset, 4);
                EXPECT_EQ(de2.vertexSpace.size, 4);

                EXPECT_EQ(de1.indexSpace.offset, 0);
                EXPECT_EQ(de1.indexSpace.size, 1);
                EXPECT_EQ(de2.indexSpace.offset, 1);
                EXPECT_EQ(de2.indexSpace.size, 1);
            }

            TEST_F(GraphicsTest, AddTooManyValuesToBatch) {
                auto dataStore = Storage<_ExampleData>::localInstance();
                auto data1 = dataStore->create();
                auto data2 = dataStore->create();
                auto data3 = dataStore->create();
                auto config = data1->getBatchConfig();
                ASSERT_NO_THROW(Batch b(2, config));
                Batch b(2, config);

                ASSERT_NO_THROW(b.add(data1));
                ASSERT_NO_THROW(b.add(data2));
                EXPECT_THROW(b.add(data3), exceptions::InsufficientBufferSpace);
            }

            TEST_F(GraphicsTest, UpdateAndRemoveFromBatch) {
                auto dataStore = Storage<_ExampleData>::localInstance();
                auto data1 = dataStore->create();
                auto data2 = dataStore->create();
                auto data3 = dataStore->create();
                auto config = data1->getBatchConfig();
                ASSERT_NO_THROW(Batch b(2, config));
                Batch b(2, config);

                Batch::Entry de1;
                Batch::Entry de2;
                ASSERT_NO_THROW(de1 = b.add(data1));
                ASSERT_NO_THROW(de2 = b.add(data2));
                EXPECT_THROW(b.add(data3), exceptions::InsufficientBufferSpace);
                b.update(de2, data2);
                b.remove(de2);

                Batch::Entry de3;
                EXPECT_NO_THROW(de3 = b.add(data3));
                EXPECT_EQ(de3.vertexSpace.offset, de2.vertexSpace.offset);
                EXPECT_EQ(de3.vertexSpace.size, de2.vertexSpace.size);
                EXPECT_EQ(de3.indexSpace.offset, de2.indexSpace.offset);
                EXPECT_EQ(de3.indexSpace.size, de2.indexSpace.size);
            }

            TEST_F(GraphicsTest, AddAndRemoveDataFromBatcher) {
                Batcher batcher(3);
                auto dataStore = Storage<_ExampleData>::localInstance();
                auto data1 = dataStore->create();
                auto data2 = dataStore->create();
                auto data3 = dataStore->create();
                batcher.set(data1);
                batcher.set(data2);
                batcher.set(data3);

                // should be only on batch
                EXPECT_NE(batcher.getBatches()->begin(), batcher.getBatches()->end());
                EXPECT_EQ(++batcher.getBatches()->begin(), batcher.getBatches()->end());

                auto data4 = dataStore->create();
                // making room in full batch
                batcher.unset(data2);
                batcher.set(data4);

                // still only one batch
                EXPECT_NE(batcher.getBatches()->begin(), batcher.getBatches()->end());
                EXPECT_EQ(++batcher.getBatches()->begin(), batcher.getBatches()->end());
            }

            TEST_F(GraphicsTest, UpdateDataInBatcher) {
                Batcher batcher(3);
                auto dataStore = Storage<_ExampleData>::localInstance();
                auto data1 = dataStore->create();
                auto data2 = dataStore->create();
                auto data3 = dataStore->create();
                batcher.set(data1);
                batcher.set(data2);
                batcher.set(data3);

                // should be only on batch
                EXPECT_NE(batcher.getBatches()->begin(), batcher.getBatches()->end());
                EXPECT_EQ(++batcher.getBatches()->begin(), batcher.getBatches()->end());

                batcher.set(data1);

                // should not have changed
                EXPECT_NE(batcher.getBatches()->begin(), batcher.getBatches()->end());
                EXPECT_EQ(++batcher.getBatches()->begin(), batcher.getBatches()->end());

                auto data4 = dataStore->create(1);
                data4->id = data3->id;
                // data4 is now data3 with a different batch config -> create additional batch
                batcher.set(data4);
                EXPECT_NE(batcher.getBatches()->begin(), batcher.getBatches()->end());
                EXPECT_EQ(++(++batcher.getBatches()->begin()), batcher.getBatches()->end());
            }

            TEST_F(GraphicsTest, RenderBatcher) {
                Batcher batcher(3);
                auto dataStore = Storage<_ExampleData>::localInstance();
                auto data1 = dataStore->create();
                auto data2 = dataStore->create();
                auto data3 = dataStore->create();
                batcher.set(data1);
                batcher.set(data2);
                batcher.set(data3);

                uint32_t counterBefore = _ExampleData::preRenderHookCount;
                batcher.render();
                // should only have one batch to render 
                EXPECT_EQ(_ExampleData::preRenderHookCount, counterBefore + 1);

                auto data4 = dataStore->create(1);
                // should create a different config -> additional batch
                batcher.set(data4);

                counterBefore = _ExampleData::preRenderHookCount;
                batcher.render();
                EXPECT_EQ(_ExampleData::preRenderHookCount, counterBefore + 2);

                Storage<VertexLayout>::global()->clear();
            }

        }
    }
}

