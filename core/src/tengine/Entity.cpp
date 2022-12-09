#include <tengine/Entity.hpp>

tengine::Entity::Entity(tengine::Color color, tengine::Transform transform, tengine::Mesh &mesh,
               mgl::ShaderProgram *shaderProgram)
            : color(color), transform(transform), mesh(mesh), shaderProgram(shaderProgram) {}


void tengine::Entity::setUniformsCallback()
{
    glm::mat4 transformM = transform.calcTransformMatrix();
    glUniformMatrix4fv(shaderProgram->Uniforms["Matrix"].index, 1, GL_FALSE, glm::value_ptr(transformM));
    glUniform4f(shaderProgram->Uniforms["inColor"].index, color.RGBA[0],
                color.RGBA[1],
                color.RGBA[2],
                color.RGBA[3]);
}

void tengine::Entity::draw()
{
    mesh.setEntity(this);
    shaderProgram->bind();
    mesh.draw();
    shaderProgram->unbind();
}