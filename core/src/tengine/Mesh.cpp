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
        glDeleteVertexArrays(1, &vaoId);
        glBindVertexArray(0);
    }

    glDeleteBuffers(2, vboId);
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