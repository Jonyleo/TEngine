#ifndef __ENTITY_HPP__
#define __ENTITY_HPP__

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "mglShader.hpp"

#include "DataStructs.hpp"
#include "Transform.hpp"
#include "Mesh.hpp"

namespace tengine
{
    class Entity
    {
    private:
        tengine::Color color;
        tengine::Transform transform;
        tengine::Mesh mesh;
        mgl::ShaderProgram *shaderProgram;

    public:
        Entity(tengine::Color color, tengine::Transform transform, tengine::Mesh &mesh,
               mgl::ShaderProgram *shaderProgram);

        void setUniformsCallback();

        void draw();
    };
}
#endif