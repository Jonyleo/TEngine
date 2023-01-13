#ifndef __DOOR_HPP__
#define __DOOR_HPP__

#include <iostream>
#include <ctime>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <memory>
#include <cassert>

#include <nlohmann/json.hpp>

#include <GL/glew.h>
#include <glm/gtc/quaternion.hpp>

#include <tengine/mglApp.hpp>
#include <tengine/mglCamera.hpp>
#include <tengine/Resources.hpp>
#include <tengine/Entity.hpp>
#include <tengine/Transform.hpp>
#include <tengine/Component.hpp>
#include <tengine/Input.hpp>
#include <tengine/mglMesh.hpp>
#include <tengine/MeshInstance.hpp>

namespace door
{
    using json = nlohmann::json;

    class DoorScript : public tengine::Component {
        private:
            std::shared_ptr<tengine::Entity> doorPivot;
            glm::quat rotation = tengine::Transform::quaternionFromAxis(glm::vec3(0.0f, 90.0f, 0.0f));

            
            bool rotate = false;

            double moveX = 0;
            double moveY = 0;

        public:
            DoorScript(tengine::Entity &parent) : tengine::Component(parent) {}


            void update(double elapsedTime) {
                if(!rotate)
                    return;

                float movement = moveX * elapsedTime * 100;

                moveX = 0;
                moveY = 0;
                
                doorPivot->getComponent<tengine::Transform>()
                    ->rotateBy(glm::mix(glm::quat(1.0f, 0.0f, 0.0f, 0.0f), rotation, movement));
            }
            
            void init() {
                for(auto &child : mgl::Engine::getInstance().getScene()->getRoot().getChildren()) {
                    if(child->getName().compare("doorPivot") == 0) {
                        doorPivot = child;
                    }
                }
                assert(doorPivot != nullptr);

                tengine::InputManager::getInstance().registerMouseDrag(GLFW_MOUSE_BUTTON_1,
                    [this] (tengine::MouseDragInput input) {

                        rotate = !input.finish;
                        if(rotate) {
                            moveX += input.x;
                            moveY += input.y;

                        } else {
                            moveX = 0;
                            moveY = 0;
                        }
                    }
                );
            }

            static std::shared_ptr<Component> loadScript(tengine::Entity &entity, json &data) {
                return std::make_shared<DoorScript>(entity);
            };

            std::string getId() { return "doorScript"; }
    };

};

#endif