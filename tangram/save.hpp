#ifndef __SAVE_HPP__
#define __SAVE_HPP__

#include <iostream>
#include <ctime>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <memory>

#include <nlohmann/json.hpp>

#include <GL/glew.h>

#include <tengine/mglApp.hpp>
#include <tengine/mglCamera.hpp>
#include <tengine/Resources.hpp>
#include <tengine/Entity.hpp>
#include <tengine/Transform.hpp>
#include <tengine/Component.hpp>
#include <tengine/Input.hpp>
#include <tengine/mglMesh.hpp>
#include <tengine/MeshInstance.hpp>

namespace script {
    using json = nlohmann::json;

    class SaveScript : public tengine::Component {
        private:
            bool doSave = false;
            int loadFile = -1;
            
            void handleLoadFile(int key) {
                tengine::InputManager::getInstance().registerKey(key,
                    [this, key] (tengine::KeyInput input) {
                        if(input.press) {
                            loadFile = key - GLFW_KEY_1;
                        };
                    }
                );
            }


        public:
            void init() {
                tengine::InputManager::getInstance().registerKey(GLFW_KEY_S,
                    [this] (tengine::KeyInput input) {
                        if(input.press) {
                            doSave = true;
                        };
                    }
                );

                handleLoadFile(GLFW_KEY_1);
                handleLoadFile(GLFW_KEY_2);
                handleLoadFile(GLFW_KEY_3);
                handleLoadFile(GLFW_KEY_4);
                handleLoadFile(GLFW_KEY_5);
                handleLoadFile(GLFW_KEY_6);
                handleLoadFile(GLFW_KEY_7);
                handleLoadFile(GLFW_KEY_8);
                handleLoadFile(GLFW_KEY_9);
            }

            void update(double elapsedTime) {
                if(loadFile != -1) {
                    std::string path = "saves";
                    std::vector<std::string> saves;
                    for(const auto & save : std::filesystem::directory_iterator(path)) {
                        saves.push_back(save.path().string());
                    }

                    if(loadFile < saves.size()) {
                        std::sort(saves.begin(), saves.end(), std::greater<std::string>());

                        std::string path = saves[loadFile];

                        mgl::Engine::getInstance().loadScene(path);
                    }

                    loadFile = -1;
                }
                if(doSave) {
                    std::string saveName = std::to_string(std::time(0));
                    tengine::ResourceManager::getInstance().save(
                        mgl::Engine::getInstance().getScene(),
                        saveName
                    );

                    doSave = false;
                }
            }


            SaveScript(tengine::Entity &entity, json &data) : Component(entity) {}

            static std::shared_ptr<Component> loadScript(tengine::Entity &entity, json &data) {
                return std::make_shared<SaveScript>(entity, data);
            };


        std::string getId() { return "saveScript"; }

        void saveComponent(json &data) { }
    };
}

#endif