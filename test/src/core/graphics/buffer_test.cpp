#include "core/graphics/base.hpp"

#include "core/graphics/buffer.hpp"

namespace tme {
    namespace core {
        namespace graphics {

            class BufferTest : public GraphicsTest {
                protected :
                struct Pair {
                    float f1, f2;
                    static size_t size() { return sizeof(Pair); }
                    bool operator==(const Pair& other) const { return this->f1 == other.f1 && this->f2 == other.f2; }
                    bool operator!=(const Pair& other) const { return !(*this == other); }
                };
                size_t m_bufferSize = 4;
                Pair m_data[4];
                Pair m_bufferData[4];
                Buffer* m_buffer;
                void SetUp() override {
                    GraphicsTest::SetUp();
                    m_buffer = new Buffer(GL_ARRAY_BUFFER, Pair::size(), m_bufferSize);
                }

                void TearDown() override {
                    delete m_buffer;
                    GraphicsTest::TearDown();
                }

                void populateBufferData() {
                    m_buffer->bind();
                    glGetBufferSubData(GL_ARRAY_BUFFER, 0, m_bufferSize * Pair::size(), m_bufferData);
                }
            };

            TEST_F(BufferTest, SequentialAdd) {
                m_data[0] = { 1.0f, 2.0f };
                auto space = m_buffer->add(1, m_data);
                EXPECT_EQ(space.offset, 0);
                EXPECT_EQ(space.size, 1);
                m_data[0] = { 1.0f, 2.0f };
                m_data[1] = { 3.0f, 4.0f };
                m_data[2] = { 5.0f, 6.0f };
                space = m_buffer->add(3, m_data);
                EXPECT_EQ(space.offset, 1);
                EXPECT_EQ(space.size, 3);

                // verify content
                populateBufferData();
                EXPECT_EQ(m_bufferData[0], m_data[0]);
                EXPECT_EQ(m_bufferData[1], m_data[0]);
                EXPECT_EQ(m_bufferData[2], m_data[1]);
                EXPECT_EQ(m_bufferData[3], m_data[2]);
            }

            TEST_F(BufferTest, AddAddRemoveAdd) {
                m_data[0] = { 1.0f, 2.0f };
                m_data[1] = { 3.0f, 4.0f };
                auto space1 = m_buffer->add(2, m_data);
                EXPECT_EQ(space1.offset, 0);
                EXPECT_EQ(space1.size, 2);

                m_data[2] = { 5.0f, 6.0f };
                m_data[3] = { 7.0f, 8.0f };
                auto space2 = m_buffer->add(2, m_data + 2);
                EXPECT_EQ(space2.offset, 2);
                EXPECT_EQ(space2.size, 2);

                m_buffer->remove(space1);

                // should have space for 2 more
                EXPECT_EQ(m_buffer->getFreeSpace(), 2);

                // should be inserted before the last buffered content
                m_data[0] = { 9.0f,10.0f };
                m_data[1] = {11.0f,12.0f };
                auto space3 = m_buffer->add(2, m_data);
                EXPECT_EQ(space3.offset, 0);
                EXPECT_EQ(space3.size, 2);

                // verify content
                populateBufferData();
                EXPECT_EQ(m_bufferData[0], m_data[0]);
                EXPECT_EQ(m_bufferData[1], m_data[1]);
                EXPECT_EQ(m_bufferData[2], m_data[2]);
                EXPECT_EQ(m_bufferData[3], m_data[3]);
            }

            TEST_F(BufferTest, FullBuffer) {
                m_data[0] = { 1.0f, 2.0f };
                m_data[1] = { 3.0f, 4.0f };
                auto space1 = m_buffer->add(2, m_data);
                EXPECT_EQ(space1.offset, 0);
                EXPECT_EQ(space1.size, 2);

                // should have space for 2 more
                EXPECT_EQ(m_buffer->getFreeSpace(), 2);

                m_data[2] = { 5.0f, 6.0f };
                m_data[3] = { 7.0f, 8.0f };
                auto space2 = m_buffer->add(2, m_data + 2);
                EXPECT_EQ(space2.offset, 2);
                EXPECT_EQ(space2.size, 2);

                EXPECT_EQ(m_buffer->getFreeSpace(), 0);

                // should return an INVALID_OFFSET because the buffer is full
                m_data[0] = { 9.0f,10.0f };
                m_data[1] = {11.0f,12.0f };
                auto space3 = m_buffer->add(2, m_data);
                EXPECT_EQ(space3.offset, INVALID_OFFSET);
                EXPECT_EQ(space3.size, 2);

                // verify content
                populateBufferData();
                EXPECT_NE(m_bufferData[0], m_data[0]);
                EXPECT_NE(m_bufferData[1], m_data[1]);
                EXPECT_EQ(m_bufferData[2], m_data[2]);
                EXPECT_EQ(m_bufferData[3], m_data[3]);
            }

            TEST_F(BufferTest, StringRepresentation) {
                std::stringstream ss;
                ss << "Buffer(" << m_buffer->getId() << ',' << GL_ARRAY_BUFFER << ',' << Pair::size() << ',' <<  m_bufferSize << ')';
                EXPECT_STREQ(m_buffer->toString().c_str(), ss.str().c_str());
            }

        } 
    }
}

