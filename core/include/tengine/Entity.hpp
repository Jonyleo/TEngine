#ifndef __ENTITY_HPP__
#define __ENTITY_HPP__

#include <memory>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>

#include "Component.hpp"

namespace tengine
{
    class Component;

    using json = nlohmann::json;
    using pComponent = std::shared_ptr<Component>;

    class Entity
    {
    private:
        std::string id;

        std::vector<pComponent> components;
        std::vector<std::shared_ptr<Entity>> children;

        Entity *parent = nullptr;

    public:
        bool visible = true;

        Entity(std::string &id) : id(id) {}

        static std::shared_ptr<Entity> load(json &entData);
        void save(json &entData);

        void attachComponent(pComponent component);
        void addChild(std::shared_ptr<Entity> child);
        void setParent(Entity *parent);
        Entity &getParent();
        bool hasParent();
        std::string getName();
        std::vector<std::shared_ptr<Entity>> getChildren();

        void draw();
        void init();
        void update(double timeElapsed);
        void finish();

        template <typename T>
        std::shared_ptr<T> getComponent()
        {
            for (auto &comp : components)
            {
                std::shared_ptr<T> ret = std::dynamic_pointer_cast<T>(comp);

                if (ret != nullptr)
                    return ret;
            }
            return nullptr;
        }
    };
}
#endif