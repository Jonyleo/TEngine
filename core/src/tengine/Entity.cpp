#include <tengine/Entity.hpp>

#include <memory>

using pComponent = std::shared_ptr<tengine::Component>;

void tengine::Entity::attachComponent(pComponent component)
{
    components.push_back(component);
}

void tengine::Entity::draw()
{
    for (auto &comp : components)
    {
        comp->bind();
    }

    for (auto &comp : components)
    {
        comp->preDraw();
    }

    for (auto &comp : components)
    {
        comp->draw();
    }

    for (auto &comp : components)
    {
        comp->postDraw();
    }

    for (auto &comp : components)
    {
        comp->unbind();
    }
}