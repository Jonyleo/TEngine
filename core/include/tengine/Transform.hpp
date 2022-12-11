#ifndef __TRANSFORM_HPP__
#define __TRANSFORM_HPP__

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "tengine.hpp"

namespace tengine
{
    class Transform : public tengine::Component
    {
    private:
        float rotation; // radians
        float scale;
        glm::vec2 position; // clipSpace

        // transformMatrix Cache
        bool changed = true;
        float lastScale = -1;
        glm::mat4 transformMatrix;

    public:
        Transform() : Transform({0, 0}, 0, 1) {}
        Transform(glm::vec2 position, float rotation, float scale) :
            position(position), rotation(rotation), scale(scale) {}

        void rotateTo(float angle);
        void rotateBy(float angle);
        void moveTo(glm::vec2 destination);
        void moveBy(glm::vec2 direction);
        void scaleTo(float scale);
        void scaleBy(float factor);
        glm::mat4 &calcTransformMatrix();
        virtual void preDraw();
    };
}
#endif