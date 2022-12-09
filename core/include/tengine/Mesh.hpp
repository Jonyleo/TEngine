#ifndef __MESH_HPP__
#define __MESH_HPP__

#include <GL/glew.h>

#include "DataStructs.hpp"

namespace tengine
{
    class Mesh
    {
    private:
        GLuint vboId[2] = {0, 0};
        GLuint vaoId = 0;
        size_t elements = 0;
        void *entity;

    public:
        Mesh(size_t n_triangles);
        ~Mesh();

        void setEntity(void *entity);
        void createVertexBuffer(const void *data, size_t n_vertexes);
        void createIndexBuffer(const void *data, size_t n_indexes);
        void createArrayBuffer(tengine::VertexAttrInfo * vertexAttrs, size_t n_vertexAttrs);
        void bind();
        void unbind();
        void draw();
    };
}
#endif