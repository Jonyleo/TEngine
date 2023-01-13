#ifndef __COMPONENT_HPP__
#define __COMPONENT_HPP__

#include <nlohmann/json.hpp>
#include "Entity.hpp"

namespace tengine
{
    using json = nlohmann::json;

    class Entity;

    class Component
    {
    private:
        static std::map<std::string, std::function<std::shared_ptr<Component>(Entity &, json&)>> & getLoaders() {
            static std::map<std::string, std::function<std::shared_ptr<Component>(Entity &, json&)>> loaders;
            return loaders;
        }

    public: 

        static void registerComponent(std::string id, std::function<std::shared_ptr<Component>(Entity &, json &)> loader) {
            Component::getLoaders()[id] = loader;
        }
        static std::shared_ptr<Component> load(Entity &entity, std::string &id, json &data) {
            return Component::getLoaders()[id](entity, data);
        }

        virtual void save(json &data) final {
            json scriptData;
            saveComponent(scriptData);

            data[getId()] = scriptData;
        }
        

        virtual void saveComponent(json &data) {};
        virtual std::string getId() = 0;

    protected:
        tengine::Entity &entity;

    public:
        Component(tengine::Entity &entity) : entity(entity) {}

        virtual void bind(){};
        virtual void preDraw(){};
        virtual void draw(){};
        virtual void postDraw(){};
        virtual void unbind(){};

        virtual void init(){};
        virtual void update(double timeElapsed){};
        virtual void finish(){};
    };
}

#endif