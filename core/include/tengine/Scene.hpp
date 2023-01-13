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
        std::string name;
        std::shared_ptr<Entity> root;

        // Volatile
        std::shared_ptr<mgl::Camera> camera;


    public:
        Scene(std::shared_ptr<Entity> root, std::string &name);
        ~Scene(){}

        static std::shared_ptr<Scene> load(std::string &path);
        void save(std::string &save);
        void draw();
        void update(double timeElapsed);
        void finish();
        Entity &getRoot()                           { return *root;   }
        mgl::Camera &getCamera()                    { return *camera; }
        void setCamera(std::shared_ptr<mgl::Camera> camera);
    };
}

#endif