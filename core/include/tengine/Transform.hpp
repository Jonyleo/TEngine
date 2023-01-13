#ifndef __TRANSFORM_HPP__
#define __TRANSFORM_HPP__

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <nlohmann/json.hpp>

#include "Component.hpp"
#include "Entity.hpp"

namespace tengine
{
    using json = nlohmann::json;

    class Transform : public Component
    {
    protected:
        glm::quat rotation;
        glm::vec3 scale;
        glm::vec3 position;

        // transformMatrix Cache
        bool changed = true;
        bool changedSinceLastUpdate = true;
        float lastScale = -1;
        glm::mat4 transformMatrix;
        glm::mat4 worldTransformMatrix;

    public:
        Transform(Entity &parent) : Transform(parent, {0, 0, 0}, glm::quat({1.0f, 0, 0, 0}), {1, 1, 1}) {}
        Transform(Entity &parent, glm::vec3 position, glm::quat rotation, glm::vec3 scale) :
            Component(parent),
            position(position),
            rotation(rotation),
            scale(scale),
            transformMatrix(1.0f){
        }

        static glm::quat quaternionFromAxis(glm::vec3 rotation);
        static glm::mat4 calcTransformMatrix(glm::vec3 position, glm::quat rotation, glm::vec3 scale);

        glm::quat getRotation()                             { return rotation; }
        glm::vec3 getPosition()                             { return position; }
        glm::vec3 getScale()                                { return scale; }
        bool didChangeSinceLastUpdate()                     { return changedSinceLastUpdate; }

        void rotateTo(glm::vec3 angles);
        void rotateTo(glm::quat angles);
        void rotateBy(glm::quat angle);
        void moveTo(glm::vec3 destination);
        void moveBy(glm::vec3 direction);
        void scaleTo(glm::vec3 scale);
        void scaleBy(glm::vec3 factor);
        glm::mat4 &getTransformMatrix();
        glm::mat4 &getWorldTransformMatrix();
        virtual void preDraw();
        virtual void finish();
        

        static std::shared_ptr<Component> loadTransform(Entity &parent, json &entData);
        virtual void saveComponent(json &entData);

        std::string getId() { return "transform"; }
    };
}
#endif