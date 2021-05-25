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

        /**//**
         * \brief Generate uuids for type T.
         *
         * @return unique id for type T
         */
        template<typename T>
        Identifier uuid() {
            static Identifier id = 0;
            return id++;
        }

        /**//**
         * \brief Interface to identify an object by id.
         */
        class Mappable {
            public:
            /**//**
             * \brief Get unique identifier of object.
             *
             * @return unique Identifier
             */
            virtual Identifier getId() const = 0;
        };

        /**//**
         * \brief General map-like storage container for classes implementing the Mappable interface.
         *
         * Allows to create instances of T which are accessible by a direct handle or indirect id.
         */
        template<typename T>
        class Storage {
            using Container = std::unordered_map<Identifier, Handle<T>>;
            Container m_data;

            /**//**
             * \brief Create storage container.
             */
            Storage() : m_data() {}

            public:
            /**//**
             * \brief Get handle for global instance of Storage<T>.
             *
             * @return Handle to global Storage of T
             */
            static Handle<Storage> global() {
                static Handle<Storage> storage = localInstance();
                return storage;
            }

            /**//**
             * \brief Create local instance of Storage<T> and get handle.
             *
             * @return Handle to local Storage of T
             */
            static Handle<Storage> localInstance() {
                return std::shared_ptr<Storage>(new Storage<T>());
            }

            /**//**
             * \brief Emplace instance of T into the Storage.
             *
             * @param args arguments used for construction of instance
             *
             * @return Handle<T> to created object
             */
            template<typename ...Args>
            Handle<T> create(Args... args) {
                T* object = new T(args...);
                return add(object);
            }

            /**//**
             * \brief Insert instance of T into the Storage
             *
             * Transfers ownership of the object to the storage
             *
             * @param object owning pointer to the object
             *
             * @return Handle<T> to inserted object
             */
            Handle<T> add(T* object)  {
                Handle<T> element(object);
                Identifier id = element->getId();
                m_data.insert({id, element});
                return element;
            }

            /**//**
             * \brief Get handle to instance of T using its id.
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

            /**//**
             * \brief Check if storage contains an element for id.
             *
             * @param id identifier of the object to be checked
             * @return true if an object exists, false if nothing is found
             */
            bool has(Identifier id) {
                return m_data.find(id) != m_data.end();
            }

            /**//**
             * \brief Remove object with id id from storage.
             *
             * @param id identifier of the object to be checked
             */
            void destroy(Identifier id) {
                m_data.erase(id);
            }

            /**//**
             * \brief Clear the storage.
             *
             * Removes all elements.
             */
            void clear() {
                m_data.clear();
            }

            /// const iterator for data inside Storage
            using const_iterator = typename Container::const_iterator;

            /**//**
             * \brief Get iterator of the start of the underlying container.
             *
             * @return begin iterator to storage
             */
            const_iterator begin() const noexcept { return m_data.begin(); }
            /**//**
             * \brief Get iterator of the end of the underlying container.
             *
             * @return end iterator to storage
             */
            const_iterator end() const noexcept { return m_data.end(); }

        };

    }
}

#endif

