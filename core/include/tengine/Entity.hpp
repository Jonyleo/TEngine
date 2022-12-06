#ifndef __ENTITY_HPP__
#define __ENTITY_HPP__

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "Mesh.hpp"

#include "mglShader.hpp"
#include "Color.hpp"
#include "Transform.hpp"

namespace tengine {
    class Entity {
        public:
            tengine::Color color;
            tengine::Transform transform;
        private:
            tengine::Mesh mesh;
            mgl::ShaderProgram* shaderProgram;

        public:
            Entity(tengine::Color color, tengine::Transform transform, tengine::Mesh& mesh,
                    mgl::ShaderProgram* shaderProgram) 
                : color(color), transform(transform), mesh(mesh),  shaderProgram(shaderProgram) {}

            void setUniforms() {
                glm::mat4 transformM = transform.calcTransformMatrix();
                glUniformMatrix4fv(shaderProgram->Uniforms["Matrix"].index, 1, GL_FALSE, glm::value_ptr(transformM));
                glUniform4f(shaderProgram->Uniforms["inColor"].index, color.RGBA[0], 
                                                                     color.RGBA[1],
                                                                     color.RGBA[2],
                                                                     color.RGBA[3]);
            }

            void draw() {
                mesh.setEntity(this);
                shaderProgram->bind();
                mesh.draw();
                shaderProgram->unbind();
            }

    };
}
#endif