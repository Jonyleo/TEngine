#ifndef __COMMON_HPP__
#define __COMMON_HPP__

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

    class CommonScript : public tengine::Component {
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
                tengine::InputManager::getInstance().registerKey(GLFW_KEY_ESCAPE,
                    [this] (tengine::KeyInput input) {
                        mgl::Engine::getInstance().close();
                    }
                );
            }


            CommonScript(tengine::Entity &entity, json &data) : Component(entity) {}

            static std::shared_ptr<Component> loadScript(tengine::Entity &entity, json &data) {
                return std::make_shared<CommonScript>(entity, data);
            };


        std::string getId() { return "commonScript"; }

        void saveComponent(json &data) { }
    };
}

#endif