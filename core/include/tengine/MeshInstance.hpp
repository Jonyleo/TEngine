#ifndef __MESHINSTANCE_HPP__
#define __MESHINSTANCE_HPP__

#include <memory>

#include "tengine.hpp"

namespace tengine
{
    class MeshInstance : public Component
    {
    private:
        tengine::Color color;
        std::shared_ptr<mgl::ShaderProgram> shader;
        std::shared_ptr<tengine::Mesh> mesh;

    public:
        MeshInstance(tengine::Color color,
                     std::shared_ptr<mgl::ShaderProgram> shader,
                     std::shared_ptr<tengine::Mesh> mesh)
            : color(color), shader(shader), mesh(mesh) {}

        virtual void bind();
        virtual void preDraw();
        void draw();
        virtual void unbind();
    };
}

#endif