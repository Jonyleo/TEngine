#ifndef __SCENE_HPP__
#define __SCENE_HPP__

#include <memory>
#include <string>

#include "Entity.hpp"

namespace tengine {
    class Scene {
        private:
            std::shared_ptr<tengine::Entity> root;

        public:
            Scene(std::shared_ptr<tengine::Entity> root);
            ~Scene() {}

            static std::shared_ptr<Scene> load(std::string &name);

            void draw();
            void update(double timeElapsed); // Todo Delta time

            tengine::Entity& getRoot() { return *root; }
    };
}

#endif