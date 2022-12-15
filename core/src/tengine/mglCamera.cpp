#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "tengine/mglCamera.hpp"
#include "tengine/mglApp.hpp"

#include <iostream>
#include <algorithm>

namespace mgl
{
    
    ///////////////////////////////////////////////////////////////////////// CameraBuffer

    CameraBuffer::CameraBuffer(GLuint bindingpoint)
        : ViewMatrix(glm::mat4(1.0f)), ProjectionMatrix(glm::mat4(1.0f))
    {
        glGenBuffers(1, &UboId);
        glBindBuffer(GL_UNIFORM_BUFFER, UboId);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, 0, GL_STREAM_DRAW);
        glBindBufferBase(GL_UNIFORM_BUFFER, bindingpoint, UboId);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    CameraBuffer::~CameraBuffer()
    {
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        glDeleteBuffers(1, &UboId);
    }

    glm::mat4 CameraBuffer::getViewMatrix() { return ViewMatrix; }

    void CameraBuffer::setViewMatrix(const glm::mat4 &viewmatrix)
    {
        ViewMatrix = viewmatrix;
        glBindBuffer(GL_UNIFORM_BUFFER, UboId);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4),
                        glm::value_ptr(ViewMatrix));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    glm::mat4 CameraBuffer::getProjectionMatrix() { return ProjectionMatrix; }

    void CameraBuffer::setProjectionMatrix(const glm::mat4 &projectionmatrix)
    {
        ProjectionMatrix = projectionmatrix;
        glBindBuffer(GL_UNIFORM_BUFFER, UboId);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4),
                        glm::value_ptr(ProjectionMatrix));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void Camera::reComputeProjection() {
        float aspectRatio = mgl::Engine::getInstance().WindowWidth / (float)mgl::Engine::getInstance().WindowHeight;
        glm::mat4 proj = perspective ? glm::perspective(fov, aspectRatio, zNear, zFar)
                                     : glm::ortho(-aspectRatio*2, aspectRatio*2, -2.0f, 2.0f, zNear, zFar);

        cameraBuff->setProjectionMatrix(proj);
    }

    void Camera::reComputeView() {
        glm::mat4 view = glm::lookAt(this->getPosition(), {0.0f,0.0f,0.0f},{0.0f, 1.0f, 0.0f});
        
        cameraBuff->setViewMatrix(view);
    }


    void Camera::zoomBy(float zoomDistance) 
    {
        radius -= zoomDistance;
        radius = std::clamp(radius, 3.0f, 9.0f);

        reComputeView();
    }

    void Camera::rotate(float deltaRotY, float deltaRotRight) {
        rotY += deltaRotY;
        rotRight += deltaRotRight;

        rotRight = std::clamp(rotRight, glm::radians(-70.0f), glm::radians(70.0f));
        
        reComputeView();
    }

    glm::vec3 Camera::getPosition() {
        glm::quat rotYQuat = glm::angleAxis(rotY, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::vec3 afterRotYposition = rotYQuat * glm::vec3({radius, 0.0f, 0.0f});

        glm::vec3 rightAxis = glm::normalize(glm::cross(afterRotYposition, glm::vec3(0.0f, 1.0f, 0.0f)));
        glm::quat rotRightQuat = glm::angleAxis(rotRight, rightAxis);

        return rotRightQuat * afterRotYposition;
    }

    ////////////////////////////////////////////////////////////////////////////////
} // namespace mgl
