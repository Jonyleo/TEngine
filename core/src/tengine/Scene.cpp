#include <fstream>
#include <memory>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <tengine/Scene.hpp>
#include <tengine/Resources.hpp>
#include <tengine/Entity.hpp>
#include <tengine/Transform.hpp>
#include <tengine/mglConventions.hpp>

namespace tengine
{
    Scene::Scene(std::shared_ptr<Entity> root) : root(root)
    {
        camera = std::make_shared<mgl::Camera>(mgl::CAMERA_BLOCK_BINDING_POINT);
        camera->setViewMatrix(glm::mat4(1.0f));
    }

    void Scene::draw()
    {
        this->root->draw();
    }

    void Scene::update(double timeElapsed)
    {
        this->root->update(timeElapsed);
    }

    std::shared_ptr<Scene> Scene::load(std::string &name)
    {
        std::ifstream sceneFile("assets/scenes/" + name + ".json");
        json sceneData = json::parse(sceneFile);

        std::string rootName = sceneData["root"];

        std::shared_ptr<Entity> root =
            ResourceManager::getInstance().load<Entity>(rootName);

        return std::make_shared<Scene>(root);
    }
}