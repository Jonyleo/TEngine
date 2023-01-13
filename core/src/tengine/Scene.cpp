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
    Scene::Scene(std::shared_ptr<Entity> root, std::string &name) : root(root), name(name) {}

    void Scene::draw(){
        this->root->draw();
    }

    void Scene::update(double timeElapsed){
        this->root->update(timeElapsed);
    }

    void Scene::finish() {
        this->root->finish();
    }

    std::shared_ptr<Scene> Scene::load(std::string &path)
    {
        std::ifstream sceneFile(path);
        json sceneData = json::parse(sceneFile);
        std::shared_ptr<Entity> root = Entity::load(sceneData["root"]);

        std::string sceneId = sceneData["id"].get<std::string>();

        return std::make_shared<Scene>(root, sceneId);
    }

    void Scene::save(std::string &save) {
        json sceneData;
        json rootData;
        root->save(rootData);
        sceneData["root"] = rootData;
        sceneData["id"] = name;

        std::ofstream sceneFile("saves/" + save + "_" + name + ".json");
        sceneFile << sceneData;
        sceneFile.close();
    }

    void Scene::setCamera(std::shared_ptr<mgl::Camera> camera)
    {
        this->camera = camera;
        this->camera->reComputeProjection();
        this->camera->reComputeView();
    }
}