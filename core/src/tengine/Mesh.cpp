#include <tengine/Mesh.hpp>
#include <tengine/Entity.hpp>

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
        // TODO disable attributes ???
        glDeleteVertexArrays(1, &vaoId);
        glBindVertexArray(0);
    }
}

void tengine::Mesh::setEntity(void *entity)
{
    this->entity = entity;
}

void tengine::Mesh::createVertexBuffer(const void *data, size_t n_vertexes)
{
    glBindBuffer(GL_ARRAY_BUFFER, vboId[0]);
    glBufferData(GL_ARRAY_BUFFER, n_vertexes, data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void tengine::Mesh::createIndexBuffer(const void *data, size_t n_indexes)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboId[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, n_indexes, data, GL_STATIC_DRAW);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void tengine::Mesh::createArrayBuffer(tengine::VertexAttrInfo * vertexAttrs, size_t n_vertexAttrs)
{
    glGenVertexArrays(1, &vaoId);
    glBindVertexArray(vaoId);

    glBindBuffer(GL_ARRAY_BUFFER, vboId[0]);

    for(int i = 0; i < n_vertexAttrs; ++i) {
        auto& attr = vertexAttrs[i];

        glEnableVertexAttribArray(attr.index);
        glVertexAttribPointer(attr.index, attr.size, attr.type, attr.normalized, attr.stride, attr.offset);
    }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glDeleteBuffers(2, vboId);
}

void tengine::Mesh::bind() { glBindVertexArray(vaoId); }

void tengine::Mesh::unbind() { glBindVertexArray(0); }

void tengine::Mesh::draw()
{
    bind();

    ((tengine::Entity *)entity)->setUniformsCallback();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboId[1]);
    glDrawElements(GL_TRIANGLES, elements, GL_UNSIGNED_BYTE, (GLvoid *)0);

    unbind();
}