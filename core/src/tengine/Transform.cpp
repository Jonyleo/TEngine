#include <tengine/tengine.hpp>

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
    scale = scale;
    changed = true;
}
void tengine::Transform::scaleBy(float factor)
{
    scale *= factor;
    changed = true;
}

glm::mat4& tengine::Transform::calcTransformMatrix()
{
    float xScale = mgl::Engine::getInstance().WindowWidth / (float) mgl::Engine::getInstance().WindowHeight;

    if(xScale == lastScale && !changed)
        return transformMatrix;

    std::cout << xScale << std::endl;
    glm::mat4 translationM = glm::translate(glm::vec3(position[0] / xScale, position[1], 0.0f));
    glm::mat4 rotationM = glm::rotate(rotation, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 scaleM = glm::scale(glm::vec3(scale / xScale, scale, 1.0f));


    transformMatrix = translationM * (rotationM * scaleM);
    
    /*glm::translate(
        glm::rotate(
            glm::scale(glm::vec3(scale / xScale, scale, 1.0f)),
            rotation,
            glm::vec3(0.0f, 0.0f, 1.0f) 
        ),
        glm::vec3(position, 0.0f)
    );*/
    

    changed = false;
    lastScale = xScale;

    return transformMatrix;
}