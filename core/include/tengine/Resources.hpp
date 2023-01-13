#ifndef __RESOURCES_HPP__
#define __RESOURCES_HPP__

#include <string>
#include <memory>
#include <map>
#include <vector>
#include <typeindex>
#include <fstream>

#include <nlohmann/json.hpp>

namespace tengine
{
    using json = nlohmann::json;

    template <class T>
    concept IsResource = requires(T a, std::string name) {
        { T::load(name) } -> std::same_as<std::shared_ptr<T>>;
    };

    template <class T>
    concept IsSavable = IsResource<T> && requires(T a, std::string data) {
        { a.save(data) };
    };

    class ResourceManager
    {
    private:
        std::map<std::type_index, std::map<std::string, std::shared_ptr<void>>> resourceCache;
        ResourceManager() {}

    protected:
        ~ResourceManager() {}

    public:
        static ResourceManager &getInstance()
        {
            static ResourceManager instance;
            return instance;
        }

        template <typename T> requires IsResource<T>
        std::shared_ptr<T> load(std::string &name)
        {
            std::type_index index = std::type_index(typeid(T));
            if (!resourceCache.contains(index))
                resourceCache[index] = std::map<std::string, std::shared_ptr<void>>();

            if (!resourceCache[index].contains(name)) {
                resourceCache[index][name] = T::load(name);
            }
            return std::static_pointer_cast<T>(resourceCache[index][name]);
        }

        template <typename T> requires IsSavable<T>
        std::shared_ptr<T> loadFromSave(std::string &name)
        {
            return T::loadFromSave(name, save);
        }

        template <typename T> requires IsSavable<T>
        void save(std::shared_ptr<T> obj, std::string &save)
        {
            obj->save(save);
        }

        void clear() {
            resourceCache.clear();
        }

        template <typename T> requires IsResource<T>
        void clear()
        {
            std::type_index index = std::type_index(typeid(T));
            if (resourceCache.contains(index))
                resourceCache[index].clear();
        }

        template <typename T> requires IsResource<T>
        void clear(std::string path)
        {
            std::type_index index = std::type_index(typeid(T));
            if (resourceCache.contains(index))
                resourceCache[index].erase(path);
        }

        ResourceManager(ResourceManager const &) = delete;
        void operator=(ResourceManager const &) = delete;
    };
}

#endif