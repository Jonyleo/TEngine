#include <tengine/tengine.hpp>

#include <glm/gtc/type_ptr.hpp>

tengine::ShaderManager &tengine::ShaderManager::getInstance()
{
    static tengine::ShaderManager instance;
    return instance;
}

void tengine::ShaderManager::setMat4(const std::string &name, const glm::mat4 &value)
{
    assert(currentShader != nullptr);

    GLuint index = currentShader->Uniforms[name].index;
    glUniformMatrix4fv(index, 1, GL_FALSE, glm::value_ptr(value));
}

void tengine::ShaderManager::setVec4(const std::string &name, const glm::vec4 &value)
{
    assert(currentShader != nullptr);

    GLuint index = currentShader->Uniforms[name].index;
    glUniform4fv(index, 1, glm::value_ptr(value));
}

void tengine::ShaderManager::setCurrentShader(std::shared_ptr<mgl::ShaderProgram> shader)
{
    currentShader = shader;
}