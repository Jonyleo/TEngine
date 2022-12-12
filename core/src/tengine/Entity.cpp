#include <memory>
#include <fstream>

#include <glm/glm.hpp>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <tengine/mglShader.hpp>

#include <tengine/Entity.hpp>
#include <tengine/Resources.hpp>
#include <tengine/Mesh.hpp>
#include <tengine/Transform.hpp>
#include <tengine/MeshInstance.hpp>



std::shared_ptr<tengine::Entity> tengine::Entity::load(std::string &name) {
    std::ifstream entFile("assets/entities/" + name + ".json");
    json entData = json::parse(entFile);
    
	
    std::shared_ptr<tengine::Entity> entity = std::make_shared<tengine::Entity>();

    if(entData.contains("mesh")) {
        std::string meshName = entData["mesh"].get<std::string>();
        // Load the mesh
        std::shared_ptr<tengine::Mesh> mesh = 
            tengine::ResourceManager::getInstance().load<tengine::Mesh>(meshName);

        // Get color
        json colorData = entData["color"];
        tengine::Color color = {0};
        for (int i = 0; i < 4; ++i)
        {
            color.RGBA[i] = colorData[i].get<int>() / 255.0f;
        }

        std::string shaderName = entData["shader"].get<std::string>();

        std::shared_ptr<mgl::ShaderProgram> shader = 
            tengine::ResourceManager::getInstance().load<mgl::ShaderProgram>(shaderName);

		entity->attachComponent(std::make_shared<tengine::MeshInstance>(*entity, color, shader, mesh));		
    }

    // Get position, rotation and scale
    json posData = entData["position"];
    glm::vec2 position(posData[0].get<float>(), posData[1].get<float>());
    float rotation = glm::radians(entData["rotation"].get<float>());
    float scale = entData["scale"].get<float>();

    entity->attachComponent(std::make_shared<tengine::Transform>(*entity, position, rotation, scale));

    if(entData.contains("children")) {
        json childData = entData["children"];

        for(int i = 0; i < childData.size(); ++i) {
            std::string childName = childData[i].get<std::string>();
            entity->addChild(
                tengine::ResourceManager::getInstance().load<tengine::Entity>(childName)
            );
        }
    }

    return entity;
}

using pComponent = std::shared_ptr<tengine::Component>;

void tengine::Entity::attachComponent(pComponent component)
{
    components.push_back(component);
}

void tengine::Entity::addChild(std::shared_ptr<tengine::Entity> child)
{
    children.push_back(child);

    child->setParent(this);
}

void tengine::Entity::setParent(tengine::Entity *parent) {
    this->parent = parent;
}

void tengine::Entity::draw()
{
    if(!visible)
        return;

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

    for(auto& child : children) {
        child->draw();
    }
}

void tengine::Entity::update(double timeElapsed) {
    for (auto &comp : components)
    {
        comp->update(timeElapsed);
    }

    for (auto &child: children) {
        child->update(timeElapsed);
    }
}