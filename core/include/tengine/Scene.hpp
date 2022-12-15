#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include <memory>
#include <string>

#include "Entity.hpp"
#include "mglCamera.hpp"

namespace tengine
{
    class Scene
    {
    private:
        std::shared_ptr<Entity> root;
        std::shared_ptr<mgl::Camera> camera;

    public:
        Scene(std::shared_ptr<Entity> root);
        ~Scene() {}

        static std::shared_ptr<Scene> load(std::string &name);

        void draw();
        void update(double timeElapsed); // Todo Delta time

        Entity &getRoot() { return *root; }
        mgl::Camera &getCamera() { return *camera; }
        void setCamera(std::shared_ptr<mgl::Camera> camera) { 
            this->camera = camera; 

            this->camera->reComputeProjection();
            this->camera->reComputeView();
        }
    };
}

#endif