////////////////////////////////////////////////////////////////////////////////
//
// Camera Abstraction Class
//
// (c)2022 by Carlos Martinho
//
////////////////////////////////////////////////////////////////////////////////

#ifndef MGL_CAMERA_HPP
#define MGL_CAMERA_HPP

#include <memory>

#include <nlohmann/json.hpp>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/detail/type_quat.hpp>

namespace mgl
{

    using json = nlohmann::json;

    class CameraBuffer;
    class Camera;

    ///////////////////////////////////////////////////////////////////////// Camera

    class CameraBuffer
    {
    private:
        GLuint UboId;
        glm::mat4 ViewMatrix;
        glm::mat4 ProjectionMatrix;

    public:
        explicit CameraBuffer(GLuint bindingpoint);
        ~CameraBuffer();

        CameraBuffer &getInstance();
        glm::mat4 getViewMatrix();
        void setViewMatrix(const glm::mat4 &viewmatrix);
        glm::mat4 getProjectionMatrix();
        void setProjectionMatrix(const glm::mat4 &projectionmatrix);

    };


    class Camera {
        private:
            std::shared_ptr<CameraBuffer> cameraBuff;

            float rotY, rotRight;
            float radius;
            float fov;
            float zNear, zFar;

            bool perspective = true;

        public:
            Camera(std::shared_ptr<CameraBuffer> cameraBuff, float radius, float fov, float zNear, float zFar) :
                cameraBuff(cameraBuff),
                radius(radius),
                fov(fov),
                zNear(zNear),
                zFar(zFar), 
                rotY(0.0f),
                rotRight(0.0f)
            {}

            void zoomBy(float zoomDistance);
            void rotate(float deltaRotY, float deltaRotRight);

            void switchPerspective() {
                perspective = !perspective;
                reComputeProjection();
            }

            void reComputeProjection();
            void reComputeView();

            glm::vec3 getPosition();

            void save(json &data);
    }; 

    ////////////////////////////////////////////////////////////////////////////////
} // namespace mgl

#endif /* MGL_CAMERA_HPP */
