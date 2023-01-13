#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <tengine/MeshInstance.hpp>
#include <tengine/Renderer.hpp>
#include <tengine/mglConventions.hpp>
#include <tengine/Resources.hpp>

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

    std::shared_ptr<Component> MeshInstance::loadMeshInstance(Entity &parent, json &entData) {
        std::string meshName = entData["mesh"].get<std::string>();
            
        // Load the mesh
        auto mesh = ResourceManager::getInstance().load<mgl::Mesh>(meshName);

        // Get color
        json colorData = entData["color"];

        Color color;

        for (int i = 0; i < 4; ++i)
            color.RGBA[i] = colorData[i].get<int>() / 255.0f;

        std::string shaderName = entData["shader"].get<std::string>();
        auto shader = ResourceManager::getInstance().load<mgl::ShaderProgram>(shaderName);

        return std::make_shared<MeshInstance>(parent, color, shader, mesh);
    }

    void MeshInstance::saveComponent(json &entData) {
        entData["mesh"] = mesh->getId();
        entData["color"] = {(int)(color.RGBA[0]*255), (int) (color.RGBA[1]*255), (int)(color.RGBA[2]*255), (int)(color.RGBA[3]*255)};
        entData["shader"] = shader->getId();   
    }
}