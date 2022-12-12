#include <fstream>
#include <memory>

#include <tengine/json.hpp>
using json = nlohmann::json;

#include <tengine/Scene.hpp>
#include <tengine/Resources.hpp>
#include <tengine/Entity.hpp>
#include <tengine/Transform.hpp>

tengine::Scene::Scene(std::shared_ptr<tengine::Entity> root) : root(root) {
    root->getComponent<tengine::Transform>()->setRoot(true);
}

void tengine::Scene::draw() {
    this->root->draw();
}

void tengine::Scene::update(float timeElapsed) {
    this->root->update(timeElapsed);
}

std::shared_ptr<tengine::Scene> tengine::Scene::load(std::string &name) {
    std::ifstream sceneFile("assets/scenes/" + name + ".json");
	json sceneData = json::parse(sceneFile);

    std::string rootName = sceneData["root"];

    std::shared_ptr<tengine::Entity> root = 
        tengine::ResourceManager::getInstance().load<tengine::Entity>(rootName);

    return std::make_shared<tengine::Scene>(root);
}
