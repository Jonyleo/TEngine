#ifndef __ENTITY_HPP__
#define __ENTITY_HPP__

#include <memory>
#include <vector>
#include <string>

#include "Component.hpp"

namespace tengine
{
    using pComponent = std::shared_ptr<tengine::Component>;

    class Entity
    {
    private:
        std::vector<pComponent> components;
        std::vector<std::shared_ptr<Entity>> children;
        Entity *parent = nullptr;
        std::string id;

    public:
        bool visible = true;

        Entity(std::string &id) : id(id) {}

        static std::shared_ptr<Entity> load(std::string &name);

        void attachComponent(pComponent component);
        void addChild(std::shared_ptr<Entity> child);
        void setParent(Entity *parent);
        Entity &getParent();
        bool hasParent();
        std::string getName();
        std::vector<std::shared_ptr<Entity>> getChildren();

        void draw();
        void update(double timeElapsed);
        void init();

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