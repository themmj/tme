#ifndef _CORE_MAP_H
#define _CORE_MAP_H
/** @file */

#include <memory>
#include <unordered_map>
#include <utility>

#include "core/log.hpp"

namespace tme {
    namespace core {

        using Identifier = uint64_t;

        template<typename T>
        using Handle = std::shared_ptr<T>;

        template<typename T>
        Identifier uuid() {
            static Identifier id = 0;
            return id++;
        }

        class Mappable {
            public:
            virtual Identifier getId() const = 0;
        };

        template<typename T>
        class Manager {
            std::unordered_map<Identifier, Handle<T>> m_data;

            public:
            Manager() : m_data() {}
            ~Manager() = default;

            static Handle<Manager> getGlobalInstance() {
                static Handle<Manager> manager = std::make_shared<Manager>();
                return manager;
            }

            static Handle<Manager> createInstance() {
                return std::make_shared<Manager>();
            }

            template<typename ...Args>
            Identifier create(Args... args) {
                T* object = new T(args...);
                return add(object);
            }

            Identifier add(T* object)  {
                Handle<T> element(object);
                Identifier id = element->getId();
                m_data.insert({id, element});
                return id;
            }

            Handle<T> get(Identifier id) {
                auto iter = m_data.find(id);
                if (iter != m_data.end()) {
                    return iter->second;
                }
                TME_ERROR("tried to access non existent mappable {}", id);
                return m_data.end()->second;
            }

            void destroy(Identifier id) {
                m_data.erase(id);
            }

            void clear() {
                m_data.clear();
            }
        };

    }
}

#endif

