#include <GL/glew.h>

#include <tengine/Renderer.hpp>

#include <glm/gtc/type_ptr.hpp>

namespace tengine
{
    Renderer &Renderer::getInstance()
    {
        static Renderer instance;
        return instance;
    }

    void Renderer::setMat4(const std::string &name, const glm::mat4 &value)
    {
        if (Renderer::getInstance().currentShader == nullptr)
            return;

        GLuint index = Renderer::getInstance().currentShader->Uniforms[name].index;
        glUniformMatrix4fv(index, 1, GL_FALSE, glm::value_ptr(value));
    }

    void Renderer::setVec4(const std::string &name, const glm::vec4 &value)
    {
        if (Renderer::getInstance().currentShader == nullptr)
            return;

        GLuint index = Renderer::getInstance().currentShader->Uniforms[name].index;
        glUniform4fv(index, 1, glm::value_ptr(value));
    }

    void Renderer::setCurrentShader(std::shared_ptr<mgl::ShaderProgram> shader)
    {
        Renderer::getInstance().currentShader = shader;
    }
}