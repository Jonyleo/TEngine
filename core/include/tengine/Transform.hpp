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
        glm::quat rotation;
        glm::vec3 scale;
        glm::vec3 position;

        // transformMatrix Cache
        bool changed = true;
        float lastScale = -1;
        glm::mat4 transformMatrix;

    public:
        Transform(tengine::Entity &parent) : Transform(parent, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}) {}
        Transform(tengine::Entity &parent, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
            : Component(parent), position(position), rotation(quaternionFromAxis(rotation)), scale(scale),
              transformMatrix(1.0f)
        {}

        static glm::quat quaternionFromAxis(glm::vec3 rotation);
        static glm::mat4 calcTransformMatrix(glm::vec3 position, glm::quat rotation, glm::vec3 scale);

        glm::quat getRotation() { return rotation; }
        glm::vec3 getPosition() { return position; }
        glm::vec3 getScale() { return scale; }

        void rotateTo(glm::vec3 angles);
        void rotateTo(glm::quat angles);
        void rotateBy(glm::quat angle);
        void moveTo(glm::vec3 destination);
        void moveBy(glm::vec3 direction);
        void scaleTo(glm::vec3 scale);
        void scaleBy(glm::vec3 factor);
        glm::mat4 &getTransformMatrix();
        virtual void preDraw();
    };
}
#endif