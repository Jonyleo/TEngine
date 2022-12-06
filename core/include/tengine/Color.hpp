#ifndef __COLOR_HPP__
#define __COLOR_HPP__

#include <GL/glew.h>

namespace tengine {
    struct Color {
        GLfloat RGBA[4];
    };

    const struct Color RED = {1.0f, 0.0f, 0.0f, 1.0f};
    const struct Color GREEN = {0.0f, 1.0f, 0.0f, 1.0f};
    const struct Color BLUE = {0.0f, 0.0f, 1.0f, 1.0f};
    const struct Color BLACK = {0.0f, 0.0f, 0.0f, 1.0f};
    const struct Color WHITE = {1.0f, 1.0f, 1.0f, 1.0f};
}

#endif