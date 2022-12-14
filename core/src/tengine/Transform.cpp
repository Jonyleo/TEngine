#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <tengine/mglApp.hpp>
#include <tengine/mglConventions.hpp>
#include <tengine/Transform.hpp>
#include <tengine/Renderer.hpp>

namespace tengine
{
    void Transform::rotateTo(glm::vec3 angle)
    {
        // Code inspired by handouts in fenix
        glm::vec3 axis_x = {1.0f, 0.0f, 0.0f};
        glm::vec3 axis_y = {0.0f, 1.0f, 0.0f};
        glm::vec3 axis_z = {0.0f, 0.0f, 1.0f};

        rotation = glm::angleAxis(glm::radians(angle[0]), axis_x) * glm::angleAxis(glm::radians(angle[1]), axis_y) * glm::angleAxis(glm::radians(angle[2]), axis_z);
        changed = true;
    }
    void Transform::rotateTo(glm::quat angle)
    {
        rotation = angle;
        changed = true;
    }
    void Transform::rotateBy(glm::quat angle)
    {
        rotation = angle * rotation;
        changed = true;
    }
    void Transform::moveTo(glm::vec3 destination)
    {
        position = destination;
        changed = true;
    }
    void Transform::moveBy(glm::vec3 direction)
    {
        position += direction;
        changed = true;
    }
    void Transform::scaleTo(glm::vec3 scale)
    {
        this->scale = scale;
        changed = true;
    }
    void Transform::scaleBy(glm::vec3 factor)
    {
        scale *= factor;
        changed = true;
    }

    glm::mat4 &Transform::getTransformMatrix()
    {
        // if (!changed)
        //     return transformMatrix;

        transformMatrix = Transform::calcTransformMatrix(position, rotation, scale);
        changed = false;

        if (entity.hasParent())
        {
            transformMatrix = entity.getParent().getComponent<Transform>()->getTransformMatrix() * transformMatrix;
        }

        return transformMatrix;
    }

    void Transform::preDraw()
    {
        Renderer::setMat4(mgl::MODEL_MATRIX, getTransformMatrix());
    }

    glm::quat Transform::quaternionFromAxis(glm::vec3 rotation)
    {
        glm::vec3 axis_x = {1.0f, 0.0f, 0.0f};
        glm::vec3 axis_y = {0.0f, 1.0f, 0.0f};
        glm::vec3 axis_z = {0.0f, 0.0f, 1.0f};

        glm::quat rotationQuat = glm::angleAxis(glm::radians(rotation[0]), axis_x) * glm::angleAxis(glm::radians(rotation[1]), axis_y) * glm::angleAxis(glm::radians(rotation[2]), axis_z);

        return rotationQuat;
    }

    glm::mat4 Transform::calcTransformMatrix(glm::vec3 position, glm::quat rotation, glm::vec3 scale)
    {
        glm::mat4 translationM = glm::translate(position);
        glm::mat4 rotationM = glm::toMat4(rotation);
        glm::mat4 scaleM = glm::scale(scale);

        return translationM * (rotationM * scaleM);
    }
}