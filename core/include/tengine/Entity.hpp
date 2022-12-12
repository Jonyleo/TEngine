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

    public:
        
        bool visible = true;

        Entity() {}

        static std::shared_ptr<Entity> load(std::string &name);

        void attachComponent(pComponent component);
        void addChild(std::shared_ptr<Entity> child);
        void setParent(Entity *parent);

        void draw();
        void update(double timeElapsed);

        template<typename T>
        std::shared_ptr<T> getComponent() {
            for(auto & comp : components) {
                std::shared_ptr<T> ret = std::dynamic_pointer_cast<T>(comp);

                if(ret != nullptr)
                    return ret;
            }
            return nullptr;
        }

        Entity* getParent() { return parent; }
    };
}
#endif