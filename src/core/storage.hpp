#ifndef _CORE_STORAGE_H
#define _CORE_STORAGE_H
/** @file */

#include <memory>
#include <unordered_map>
#include <utility>

#include "core/log.hpp"

namespace tme {
    namespace core {

        /// type used for identifiers throughout the codebase
        using Identifier = uint32_t;

        /// type alias for a shared ptr to T
        template<typename T>
        using Handle = std::shared_ptr<T>;

        /// generate uuids for type T
        template<typename T>
        Identifier uuid() {
            static Identifier id = 0;
            return id++;
        }

        /// pure virtual base class to identify an object by id
        class Mappable {
            public:
            /// getter for the unique id
            virtual Identifier getId() const = 0;
        };

        /**//**
         * General map-like storage container for classes implementing the Mappable interface.
         *
         * Allows to create instances of T which are accessible by a direct handle or indirect id.
         */
        template<typename T>
        class Storage {
            std::unordered_map<Identifier, Handle<T>> m_data;

            // create storage container
            Storage() : m_data() {}

            public:
            /// get handle for global instance of Storage<T>
            static Handle<Storage> global() {
                static Handle<Storage> storage = localInstance();
                return storage;
            }

            /// create local instance of Storage<T> and get handle
            static Handle<Storage> localInstance() {
                return std::shared_ptr<Storage>(new Storage<T>());
            }

            /**//**
             * emplace instance of T into the Storage
             *
             * @param args arguments used for construction of instance
             * @return Handle<T> to created object
             */
            template<typename ...Args>
            Handle<T> create(Args... args) {
                T* object = new T(args...);
                return add(object);
            }

            /**//**
             * insert instance of T into the Storage
             * transfers ownership to the object to the storage
             *
             * @param object owning pointer to the object
             * @return Handle<T> to inserted object
             */
            Handle<T> add(T* object)  {
                Handle<T> element(object);
                Identifier id = element->getId();
                m_data.insert({id, element});
                return element;
            }


            /**//**
             * get handle to instance of T using its id
             *
             * @param id identifier of the object to be accessed
             * @return Handle<T> to found object or Handle(nullptr) if nothing is found
             */
            Handle<T> get(Identifier id) {
                auto iter = m_data.find(id);
                if (iter != m_data.end()) {
                    return iter->second;
                }
                TME_WARN("tried to access non existent mappable {}, returning nullptr handle", id);
                return Handle<T>(nullptr);
            }

            /// remove object with id id from storage
            void destroy(Identifier id) {
                m_data.erase(id);
            }

            /// clear the storage, removes all elements
            void clear() {
                m_data.clear();
            }
        };

    }
}

#endif

