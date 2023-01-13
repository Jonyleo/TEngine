#include <memory>
#include <fstream>

#include <glm/glm.hpp>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <tengine/mglShader.hpp>
#include <tengine/mglMesh.hpp>

#include <tengine/Entity.hpp>
#include <tengine/Resources.hpp>
#include <tengine/Transform.hpp>
#include <tengine/MeshInstance.hpp>

namespace tengine
{

    std::shared_ptr<Entity> Entity::load(json &entData)
    {
        std::string entityId = entData["id"].get<std::string>();
        std::shared_ptr<Entity> entity = std::make_shared<Entity>(entityId);

        json compData = entData["components"];

        for(json::iterator it = compData.begin(); it != compData.end(); ++it) {
            std::string compId = it.key();
            entity->attachComponent(Component::load(*entity, compId, it.value()));
        }

        if (entData.contains("children"))
        {
            json childData = entData["children"];

            for (int i = 0; i < childData.size(); ++i)
            {
                entity->addChild(Entity::load(childData[i]));
            }
        }

        return entity;
    }

    void Entity::save(json &entData) {
        json compData;

        for(auto component : components) {
            component->save(compData);
        }

        entData["components"] = compData;

        json childrenData = json::array();

        for(auto child : children) {
            json childData;
            child->save(childData);

            childrenData.push_back(childData);
        }
        
        entData["children"] = childrenData;
        entData["id"] = id;
    }

    using pComponent = std::shared_ptr<Component>;

    void Entity::attachComponent(pComponent component)      { components.push_back(component); }

    void Entity::addChild(std::shared_ptr<Entity> child) {
        children.push_back(child);
        child->setParent(this);
    }

    void Entity::setParent(Entity *parent)                  { this->parent = parent; }

    Entity &Entity::getParent()                             { return *parent; }

    bool Entity::hasParent()                                { return parent != nullptr; }

    void Entity::draw()
    {
        if (!visible)
            return;

        for (auto &comp : components)
            comp->bind();

        for (auto &comp : components)
            comp->preDraw();

        for (auto &comp : components)
            comp->draw();

        for (auto &comp : components)
            comp->postDraw();

        for (auto &comp : components)
            comp->unbind();

        for (auto &child : children)
            child->draw();
    }

    void Entity::init()
    {
        for (auto &comp : components)
            comp->init();

        for (auto &child : children)
            child->init();
    }

    void Entity::update(double timeElapsed)
    {
        for (auto &comp : components)
            comp->update(timeElapsed);

        for (auto &child : children)
            child->update(timeElapsed);
    }

    void Entity::finish() 
    {
        for (auto &comp : components)
            comp->finish();

        for (auto &child : children)
            child->finish();

    }

    std::string Entity::getName()                                   { return id; }
    std::vector<std::shared_ptr<Entity>> Entity::getChildren()      { return children; }
}