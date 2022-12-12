#ifndef __TRANSFORM_HPP__
#define __TRANSFORM_HPP__

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Entity.hpp"

namespace tengine
{
    class Transform : public tengine::Component
    {
    protected:
        float rotation; // radians
        float scale;
        glm::vec2 position; // clipSpace

        // transformMatrix Cache
        bool changed = true;
        bool isRoot = false;
        float lastScale = -1;
        glm::mat4 transformMatrix;

    public:
        Transform(tengine::Entity &parent) : Transform(parent, {0, 0}, 0, 1) {}
        Transform(tengine::Entity &parent, glm::vec2 position, float rotation, float scale) :
            Component(parent), position(position), rotation(rotation), scale(scale) {}

        void rotateTo(float angle);
        void rotateBy(float angle);
        void moveTo(glm::vec2 destination);
        void moveBy(glm::vec2 direction);
        void scaleTo(float scale);
        void scaleBy(float factor);
        glm::mat4 &calcTransformMatrix();
        virtual void preDraw();

        void setRoot(bool value);
    };
}
#endif