#include <glm/gtc/type_ptr.hpp>

#include <tengine/mglApp.hpp>
#include <tengine/mglConventions.hpp>
#include <tengine/Transform.hpp>
#include <tengine/ShaderManager.hpp>

void tengine::Transform::rotateTo(float angle)
{
    rotation = angle;
    changed = true;
}
void tengine::Transform::rotateBy(float angle)
{
    rotation += angle;
    changed = true;
}
void tengine::Transform::moveTo(glm::vec2 destination)
{
    position = destination;
    changed = true;
}
void tengine::Transform::moveBy(glm::vec2 direction)
{
    position += direction;
    changed = true;
}
void tengine::Transform::scaleTo(float scale)
{
    this->scale = scale;
    changed = true;
}
void tengine::Transform::scaleBy(float factor)
{
    scale *= factor;
    changed = true;
}

void tengine::Transform::setRoot(bool value)
{
    isRoot = value;
    changed = true;
}

glm::mat4 &tengine::Transform::calcTransformMatrix()
{
    // if (!changed)
    //     return transformMatrix;

    glm::mat4 translationM = glm::translate(glm::vec3(position[0], position[1], 0.0f));
    glm::mat4 rotationM = glm::rotate(rotation, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 scaleM = glm::scale(glm::vec3(scale, scale, 1.0f));

    transformMatrix = translationM * (rotationM * scaleM);
    changed = false;

    if (isRoot)
    {
        float aspectRatio = mgl::Engine::getInstance().WindowWidth / (float)mgl::Engine::getInstance().WindowHeight;

        transformMatrix = glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f) * transformMatrix;
    }
    else
    {
        transformMatrix = parent.getParent()->getComponent<tengine::Transform>()->calcTransformMatrix() * transformMatrix;
    }

    return transformMatrix;
}

void tengine::Transform::preDraw()
{
    tengine::ShaderManager::getInstance().setMat4(mgl::MODEL_MATRIX, calcTransformMatrix());
}
