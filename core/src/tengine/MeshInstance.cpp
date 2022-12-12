#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <tengine/MeshInstance.hpp>
#include <tengine/ShaderManager.hpp>
#include <tengine/mglConventions.hpp>

void tengine::MeshInstance::bind()
{
    mesh->bind();
    shader->bind();

    tengine::ShaderManager::getInstance().setCurrentShader(shader);
}

void tengine::MeshInstance::preDraw()
{
    tengine::ShaderManager::getInstance().setVec4(mgl::COLOR_ATTRIBUTE, glm::make_vec4(color.RGBA));
}

void tengine::MeshInstance::draw()
{
    mesh->draw();
}

void tengine::MeshInstance::unbind()
{
    tengine::ShaderManager::getInstance().setCurrentShader(nullptr);
    mesh->unbind();
    shader->unbind();
}