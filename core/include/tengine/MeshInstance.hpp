#ifndef __MESHINSTANCE_HPP__
#define __MESHINSTANCE_HPP__

#include <memory>

#include "mglShader.hpp"

#include "mglMesh.hpp"

#include "DataStructs.hpp"
#include "Entity.hpp"

namespace tengine
{
    class MeshInstance : public Component
    {
    private:
        tengine::Color color;
        std::shared_ptr<mgl::ShaderProgram> shader;
        std::shared_ptr<mgl::Mesh> mesh;

    public:
        MeshInstance(
            tengine::Entity &parent,
            tengine::Color color,
            std::shared_ptr<mgl::ShaderProgram> shader,
            std::shared_ptr<mgl::Mesh> mesh
        ):
            Component(parent),
            color(color),
            shader(shader),
            mesh(mesh)
        {}

        virtual void bind();
        virtual void preDraw();
        void draw();
        virtual void unbind();
    };
}

#endif