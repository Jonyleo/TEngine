#ifndef __DATASTRUCTS_HPP__
#define __DATASTRUCTS_HPP__

#include <GL/glew.h>

namespace tengine
{
    struct Color
    {
        GLfloat RGBA[4] = {0};
    };

    const struct Color RED = {1.0f, 0.0f, 0.0f, 1.0f};
    const struct Color GREEN = {0.0f, 1.0f, 0.0f, 1.0f};
    const struct Color BLUE = {0.0f, 0.0f, 1.0f, 1.0f};
    const struct Color BLACK = {0.0f, 0.0f, 0.0f, 1.0f};
    const struct Color WHITE = {1.0f, 1.0f, 1.0f, 1.0f};

    struct Point
    {
        GLfloat XYZW[4] = {0};

        Point() : XYZW{{0}} {}
        Point(GLfloat x1, GLfloat x2, GLfloat x3, GLfloat x4) : XYZW{x1, x2, x3, x4} {}
    };

    struct VertexAttrInfo
    {
        GLuint index;
        GLint size;
        GLenum type;
        GLboolean normalized;
        GLsizei stride;
        const void *offset;
    };
}

#endif