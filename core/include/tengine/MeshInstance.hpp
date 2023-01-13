#ifndef __MESHINSTANCE_HPP__
#define __MESHINSTANCE_HPP__

#include <memory>

#include "mglShader.hpp"

#include "mglMesh.hpp"

#include "DataStructs.hpp"
#include "Component.hpp"
#include "Entity.hpp"

namespace tengine
{
    class MeshInstance : public Component
    {
    private:
        Color color;
        std::shared_ptr<mgl::ShaderProgram> shader;
        std::shared_ptr<mgl::Mesh> mesh;

    public:
        MeshInstance(
            Entity &parent,
            Color color,
            std::shared_ptr<mgl::ShaderProgram> shader,
            std::shared_ptr<mgl::Mesh> mesh
        ):
            Component(parent),
            color(color),
            shader(shader),
            mesh(mesh)
        {}

        MeshInstance(
            Entity &parent
        ):
            MeshInstance(parent, {0,0,0,0}, nullptr, nullptr)
        {}
        virtual void bind();
        virtual void preDraw();
        void draw();
        virtual void unbind();

        static std::shared_ptr<Component> loadMeshInstance(Entity &parent, json &entData);
        virtual void saveComponent(json &entData);

        std::string getId() { return "meshInstance"; }
    };
}

#endif