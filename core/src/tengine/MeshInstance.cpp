#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <tengine/MeshInstance.hpp>
#include <tengine/Renderer.hpp>
#include <tengine/mglConventions.hpp>

namespace tengine
{
    void MeshInstance::bind()
    {
        mesh->bind();
        shader->bind();

        Renderer::setCurrentShader(shader);
    }

    void MeshInstance::preDraw()
    {
        Renderer::setVec4(mgl::COLOR_ATTRIBUTE, glm::make_vec4(color.RGBA));
    }

    void MeshInstance::draw()
    {
        mesh->draw();
    }

    void MeshInstance::unbind()
    {
        Renderer::setCurrentShader(nullptr);
        mesh->unbind();
        shader->unbind();
    }
}