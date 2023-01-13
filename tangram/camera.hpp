#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__


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

    class CameraScript : public tengine::Component {
        private:
            const float sensitivity = 1.5;

            int nCamera = -1;

            std::vector<std::shared_ptr<mgl::Camera>> cameras;

            double zoom = 0;

            bool switchCamera = false;
            bool switchPerspective = true;

            bool rotate = false;

            double moveX = 0;
            double moveY = 0;

            bool enableDrag = true;

        public:
            void init() {
                std::shared_ptr<tengine::Scene> scene = mgl::Engine::getInstance().getScene();
		
		        scene->setCamera(cameras[nCamera]);

                tengine::InputManager::getInstance().registerKey(GLFW_KEY_C,
                    [this] (tengine::KeyInput input) {
                        if(input.press)
                            switchCamera = true;
                    }
                );

                tengine::InputManager::getInstance().registerKey(GLFW_KEY_P,
                    [this] (tengine::KeyInput input) {
                        if(input.press)
                            switchPerspective = true;
                    }
                );

                tengine::InputManager::getInstance().registerScrollWheel(
                    [this] (tengine::ScrollWheelInput input) {
                        zoom = input.delta;
                    }
                );

                if(enableDrag) {
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
            }

            void update(double elapsedTime) {
                if(switchCamera) {
                    nCamera = (static_cast<unsigned long long>(nCamera) + 1) % cameras.size();
                    mgl::Engine::getInstance().getScene()->setCamera(cameras[nCamera]);
                    switchCamera = false;
                }

                if(zoom != 0) {
                    mgl::Engine::getInstance().getScene()->getCamera().zoomBy(zoom);
                    zoom = 0;
                }

                if(enableDrag && rotate) {
                    mgl::Engine::getInstance().getScene()->getCamera()
                        .rotate(-moveX * sensitivity, moveY * sensitivity);
                    
                    moveX = 0;
                    moveY = 0;
                }

                if(switchPerspective) {
                    mgl::Engine::getInstance().getScene()->getCamera().switchPerspective();
                    switchPerspective = false;
                }
            }


            CameraScript(tengine::Entity &entity, json &data) : Component(entity) {
                nCamera = data["nCamera"].get<int>();

                json cameraData = data["cameras"];
                
                enableDrag = data["enableDrag"].get<bool>();

                for(int i = 0; i < cameraData.size(); ++i) {
                    json cam = cameraData[i];

                    cameras.push_back(
                        std::make_shared<mgl::Camera>(
                            mgl::Engine::getInstance().getCameraBuff(), 
                            cam["radius"],
                            cam["fov"],
                            cam["zNear"],
                            cam["zFar"]
                        )
                    );

                    if(cam["perspective"].get<bool>())
                        cameras[i]->switchPerspective();

                    cameras[i]->rotate(cam["rotY"], cam["rotRight"]);
                }
            }

            static std::shared_ptr<Component> loadScript(tengine::Entity &entity, json &data) {
                return std::make_shared<CameraScript>(entity, data);
            };


        std::string getId() { return "cameraScript"; }

        void saveComponent(json &data) {
            data["nCamera"] = nCamera;

            json cameraArray = json::array();

            for(auto cam : cameras) {
                json cameraData;

                cam->save(cameraData);
                cameraArray.push_back(cameraData);
            }
            data["cameras"] = cameraArray;

            data["enableDrag"] = enableDrag;
        }
    };
}

#endif