////////////////////////////////////////////////////////////////////////////////
//
// Camera Abstraction Class
//
// (c)2022 by Carlos Martinho
//
////////////////////////////////////////////////////////////////////////////////

#ifndef MGL_CAMERA_HPP
#define MGL_CAMERA_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace mgl
{

    class Camera;

    ///////////////////////////////////////////////////////////////////////// Camera

    class Camera
    {
    private:
        GLuint UboId;
        glm::mat4 ViewMatrix;
        glm::mat4 ProjectionMatrix;

        float aspectRatio;

    public:
        explicit Camera(GLuint bindingpoint);
        ~Camera();
        glm::mat4 getViewMatrix();
        void setViewMatrix(const glm::mat4 &viewmatrix);
        glm::mat4 getProjectionMatrix();
        void setProjectionMatrix(const glm::mat4 &projectionmatrix);
        void setAspectRatio(float aspectRatio);
    };

    ////////////////////////////////////////////////////////////////////////////////
} // namespace mgl

#endif /* MGL_CAMERA_HPP */
