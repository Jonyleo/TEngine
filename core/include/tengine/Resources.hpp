#ifndef __RESOURCES_HPP__
#define __RESOURCES_HPP__

#include <string>
#include <memory>
#include <map>
#include <vector>
#include <typeindex>

namespace tengine
{

    template <class T>
    concept IsResource = requires(T a, std::string name) {
                             {
                                 T::load(name)
                                 } -> std::same_as<std::shared_ptr<T>>;
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

        template <typename T>
            requires IsResource<T>
        std::shared_ptr<T> load(std::string &name)
        {
            std::type_index index = std::type_index(typeid(T));
            if (!resourceCache.contains(index))
                resourceCache[index] = std::map<std::string, std::shared_ptr<void>>();

            if (!resourceCache[index].contains(name))
                resourceCache[index][name] = T::load(name);

            return std::static_pointer_cast<T>(resourceCache[index][name]);
        }

        void clear()
        {
            resourceCache.clear();
        }

        template <typename T>
            requires IsResource<T>
        void clear()
        {
            std::type_index index = std::type_index(typeid(T));
            if (resourceCache.contains(index))
                resourceCache[index].clear();
        }

        ResourceManager(ResourceManager const &) = delete;
        void operator=(ResourceManager const &) = delete;
    };
}

#endif