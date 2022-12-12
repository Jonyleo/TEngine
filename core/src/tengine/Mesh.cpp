#include <fstream>
#include <memory>

#include <tengine/json.hpp>
using json = nlohmann::json;

#include <tengine/Mesh.hpp>
#include <tengine/Entity.hpp>
#include <tengine/DataStructs.hpp>

#include <tengine/mglConventions.hpp>

tengine::Mesh::Mesh(size_t n_triangles)
{
    glGenBuffers(2, vboId);
    elements = n_triangles;
}

tengine::Mesh::~Mesh()
{
    if (vaoId != 0)
    {
        glBindVertexArray(vaoId);
        glDeleteVertexArrays(1, &vaoId);
        glBindVertexArray(0);
    }

    glDeleteBuffers(2, vboId);
}


std::shared_ptr<tengine::Mesh> tengine::Mesh::load(std::string& name) {
    std::ifstream meshFile("assets/meshes/" + name + ".json");
	json meshData = json::parse(meshFile);

    // Load vertex data
    json vertexData = meshData["vertex"];
    size_t n_vertex = vertexData.size();
    std::vector<tengine::Point> vertexes(n_vertex);
    for (int i = 0; i < n_vertex; ++i)
    {
        vertexes[i] = {vertexData[i][0].get<float>(), vertexData[i][1].get<float>(), 0.0f, 1.0f};
    }

    // Load index data
    json indexData = meshData["index"];
    size_t n_index = indexData.size();
    std::vector<GLubyte> indexes(n_index);
    for (int i = 0; i < n_index; ++i)
    {
        indexes[i] = indexData[i].get<GLuint>();
    }

    tengine::VertexAttrInfo positionAttr = {mgl::POSITION_INDEX, 4, GL_FLOAT, GL_FALSE, sizeof(tengine::Point), 0};

    // Create the mesh
    std::shared_ptr<tengine::Mesh> mesh = std::make_shared<tengine::Mesh>(n_index);
    mesh->createVertexBuffer(vertexes.data(), vertexes.size() * sizeof(tengine::Point));
    mesh->createIndexBuffer(indexes.data(), indexes.size() * sizeof(GLubyte));
    mesh->createArrayBuffer(&positionAttr, 1);
    return mesh;
}

void tengine::Mesh::createVertexBuffer(const void *data, size_t size)
{
    glBindBuffer(GL_ARRAY_BUFFER, vboId[0]);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void tengine::Mesh::createIndexBuffer(const void *data, size_t size)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboId[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void tengine::Mesh::createArrayBuffer(tengine::VertexAttrInfo *vertexAttrs, size_t n_vertexAttrs)
{
    glGenVertexArrays(1, &vaoId);
    glBindVertexArray(vaoId);

    glBindBuffer(GL_ARRAY_BUFFER, vboId[0]);

    for (int i = 0; i < n_vertexAttrs; ++i)
    {
        auto &attr = vertexAttrs[i];

        glEnableVertexAttribArray(attr.index);
        glVertexAttribPointer(attr.index, attr.size, attr.type, attr.normalized, attr.stride, attr.offset);
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void tengine::Mesh::bind()
{
    glBindVertexArray(vaoId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboId[1]);
}

void tengine::Mesh::unbind()
{
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void tengine::Mesh::draw()
{
    glDrawElements(GL_TRIANGLES, elements, GL_UNSIGNED_BYTE, (GLvoid *)0);
    //glDrawArrays(GL_TRIANGLES, 0, elements);
}