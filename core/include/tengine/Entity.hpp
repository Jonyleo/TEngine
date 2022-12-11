#ifndef __ENTITY_HPP__
#define __ENTITY_HPP__

#include <memory>
#include <vector>

#include "tengine.hpp"

namespace tengine
{
    using pComponent = std::shared_ptr<tengine::Component>;

    class Entity
    {
    private:
        std::vector<pComponent> components;

    public:
        Entity(std::vector<pComponent> components)
            : components(components) {}
        Entity() {}

        void attachComponent(pComponent component);

        void draw();
    };
}
#endif