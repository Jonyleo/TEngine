#ifndef __MESHINSTANCE_HPP__
#define __MESHINSTANCE_HPP__

#include <memory>

#include "tengine.hpp"

namespace tengine {
    class MeshInstance {
        private:
            tengine::Color color;
            std::shared_ptr<mgl::ShaderProgram> shader;
            std::shared_ptr<tengine::Mesh> mesh;

        public:
            MeshInstance(tengine::Color color, 
                         std::shared_ptr<mgl::ShaderProgram> shader,
                         std::shared_ptr<tengine::Mesh> mesh)
                : color(color), shader(shader), mesh(mesh) {}
            

            void draw();
    };
}

#endif