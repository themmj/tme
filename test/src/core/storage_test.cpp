#include "gtest/gtest.h"
#include "core/storage.hpp"

namespace tme {
    namespace core {

        class _ExampleClass : public Mappable {
            Identifier m_id;

            public:
            _ExampleClass() : m_id(uuid<_ExampleClass>()) {}
            Identifier getId() const override { return m_id; }
        };

        TEST(TestStorage, CreateMappableObject) {
            _ExampleClass ec;
            EXPECT_EQ(uuid<_ExampleClass>() - 1, ec.getId());
        }

        TEST(TestStorage, CreateInGlobal) {
            auto created = Storage<_ExampleClass>::global()->create();
            auto added = Storage<_ExampleClass>::global()->add(new _ExampleClass());

            EXPECT_NE(created->getId(), added->getId());
            EXPECT_EQ(created.get(), Storage<_ExampleClass>::global()->get(created->getId()).get());
            EXPECT_EQ(added.get(), Storage<_ExampleClass>::global()->get(added->getId()).get());
        }

        TEST(TestStorage, DeleteInGlobal) {
            auto created = Storage<_ExampleClass>::global()->create();
            auto added = Storage<_ExampleClass>::global()->add(new _ExampleClass());

            EXPECT_EQ(created.get(), Storage<_ExampleClass>::global()->get(created->getId()).get());
            EXPECT_EQ(added.get(), Storage<_ExampleClass>::global()->get(added->getId()).get());

            Storage<_ExampleClass>::global()->destroy(added->getId());
            EXPECT_EQ(created.get(), Storage<_ExampleClass>::global()->get(created->getId()).get());
            EXPECT_NE(added.get(), Storage<_ExampleClass>::global()->get(added->getId()).get());

            Storage<_ExampleClass>::global()->destroy(created->getId());
            EXPECT_NE(created.get(), Storage<_ExampleClass>::global()->get(created->getId()).get());
            EXPECT_NE(added.get(), Storage<_ExampleClass>::global()->get(added->getId()).get());
        }

        TEST(TestStorage, CreateInLocal) {
            // creating two seperate storages and check that elements are available in the respective storages
            auto createdStorage = Storage<_ExampleClass>::localInstance();
            auto addedStorage = Storage<_ExampleClass>::localInstance();

            auto created = createdStorage->create();
            auto added = addedStorage->add(new _ExampleClass());

            EXPECT_NE(created->getId(), added->getId());

            EXPECT_EQ(created.get(), createdStorage->get(created->getId()).get());
            EXPECT_EQ(added.get(), addedStorage->get(added->getId()).get());

            EXPECT_NE(added.get(), createdStorage->get(added->getId()).get());
            EXPECT_NE(created.get(), addedStorage->get(added->getId()).get());
        }

        TEST(TestStorage, DeleteInLocal) {
            auto createdStorage = Storage<_ExampleClass>::localInstance();
            auto addedStorage = Storage<_ExampleClass>::localInstance();

            auto created = createdStorage->create();
            auto added = addedStorage->add(new _ExampleClass());

            EXPECT_EQ(created.get(), createdStorage->get(created->getId()).get());
            EXPECT_EQ(added.get(), addedStorage->get(added->getId()).get());

            addedStorage->destroy(added->getId());
            EXPECT_EQ(created.get(), createdStorage->get(created->getId()).get());
            EXPECT_NE(added.get(), addedStorage->get(added->getId()).get());

            createdStorage->destroy(created->getId());
            EXPECT_NE(created.get(), createdStorage->get(created->getId()).get());
            EXPECT_NE(added.get(), addedStorage->get(added->getId()).get());
        }

        TEST(TestStorage, ClearStorage) {
            auto created = Storage<_ExampleClass>::global()->create();
            auto added = Storage<_ExampleClass>::global()->add(new _ExampleClass());

            EXPECT_EQ(created.get(), Storage<_ExampleClass>::global()->get(created->getId()).get());
            EXPECT_EQ(added.get(), Storage<_ExampleClass>::global()->get(added->getId()).get());

            Storage<_ExampleClass>::global()->clear();
            EXPECT_NE(created.get(), Storage<_ExampleClass>::global()->get(created->getId()).get());
            EXPECT_NE(added.get(), Storage<_ExampleClass>::global()->get(added->getId()).get());
        }

        TEST(TestStorage, HasElement) {
            auto storage = Storage <_ExampleClass>::localInstance();
            _ExampleClass* ptr = new _ExampleClass();
            EXPECT_FALSE(storage->has(ptr->getId()));

            storage->add(ptr);
            EXPECT_TRUE(storage->has(ptr->getId()));
        }

        TEST(TestStorage, Iterators) {
            auto storage = Storage <_ExampleClass>::localInstance();
            EXPECT_EQ(storage->begin(), storage->end());
            storage->create();
            EXPECT_NE(storage->begin(), storage->end());
        }

    }
}

