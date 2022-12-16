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

    std::shared_ptr<Entity> Entity::load(std::string &name)
    {
        std::ifstream entFile("assets/entities/" + name + ".json");
        json entData = json::parse(entFile);

        std::shared_ptr<Entity> entity = std::make_shared<Entity>(name);

        if (entData.contains("mesh"))
        {
            std::string meshName = entData["mesh"].get<std::string>();
            
            // Load the mesh
            std::shared_ptr<mgl::Mesh> mesh =
                ResourceManager::getInstance().load<mgl::Mesh>(meshName);

            // Get color
            json colorData = entData["color"];
            Color color = {0};

            for (int i = 0; i < 4; ++i)
                color.RGBA[i] = colorData[i].get<int>() / 255.0f;

            std::string shaderName = entData["shader"].get<std::string>();

            std::shared_ptr<mgl::ShaderProgram> shader =
                ResourceManager::getInstance().load<mgl::ShaderProgram>(shaderName);

            entity->attachComponent(std::make_shared<MeshInstance>(*entity, color, shader, mesh));
        }

        // Get position, rotation and scale
        json posData = entData["position"];
        glm::vec3 position(posData[0].get<float>(), posData[1].get<float>(), posData[2].get<float>());

        json rotData = entData["rotation"];
        glm::vec3 rotation(rotData[0].get<float>(), rotData[1].get<float>(), rotData[2].get<float>());

        json sclData = entData["scale"];
        glm::vec3 scale(sclData[0].get<float>(), sclData[1].get<float>(), sclData[2].get<float>());

        entity->attachComponent(std::make_shared<Transform>(*entity, position, rotation, scale));

        if (entData.contains("children"))
        {
            json childData = entData["children"];

            for (int i = 0; i < childData.size(); ++i)
            {
                std::string childName = childData[i].get<std::string>();
                entity->addChild(
                    ResourceManager::getInstance().load<Entity>(childName));
            }
        }

        return entity;
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

    std::string Entity::getName()                                   { return id; }
    std::vector<std::shared_ptr<Entity>> Entity::getChildren()      { return children; }
}