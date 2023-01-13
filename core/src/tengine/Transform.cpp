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
        changedSinceLastUpdate = true;
    }
    void Transform::rotateTo(glm::quat angle)
    {
        rotation = angle;
        changed = true;
        changedSinceLastUpdate = true;
    }
    void Transform::rotateBy(glm::quat angle)
    {
        rotation = angle * rotation;
        changed = true;
        changedSinceLastUpdate = true;
    }
    void Transform::moveTo(glm::vec3 destination)
    {
        position = destination;
        changed = true;
        changedSinceLastUpdate = true;
    }
    void Transform::moveBy(glm::vec3 direction)
    {
        position += direction;
        changed = true;
        changedSinceLastUpdate = true;
    }
    void Transform::scaleTo(glm::vec3 scale)
    {
        this->scale = scale;
        changed = true;
        changedSinceLastUpdate = true;
    }
    void Transform::scaleBy(glm::vec3 factor)
    {
        scale *= factor;
        changed = true;
        changedSinceLastUpdate = true;
    }

    glm::mat4 &Transform::getTransformMatrix()
    {
        if(changed)
            transformMatrix = Transform::calcTransformMatrix(position, rotation, scale);

        changed = false;

        return transformMatrix;
    }

    glm::mat4 &Transform::getWorldTransformMatrix() {
        bool transformChanged = changed;
        getTransformMatrix();

        if(!entity.hasParent()) {
            return transformMatrix;
        }

        bool parentChanged = entity.getParent().getComponent<tengine::Transform>()->didChangeSinceLastUpdate();


        if(parentChanged || transformChanged) {
            worldTransformMatrix = entity.getParent().getComponent<tengine::Transform>()->getWorldTransformMatrix() * transformMatrix;
            #ifdef PERF_CHECK
                static long parentCalc = 0;
                static long step = 1;
                parentCalc++;


                if(parentCalc % step == 0){
                    std::cout << "[PERF] Caculated " << parentCalc << " transforms from parent" << std::endl;
                    step *= 10;
                }
            #endif
        }
        return worldTransformMatrix;        
    }

    void Transform::preDraw()
    {
        Renderer::setMat4(mgl::MODEL_MATRIX, getWorldTransformMatrix());
    }

    void Transform::finish()
    {
        changedSinceLastUpdate = false;
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
        #ifdef PERF_CHECK
            static long transformCalc = 0;
            static long step = 1;
            transformCalc++;
                

            if(transformCalc % step == 0){
                std::cout << "[PERF] Caculated " << transformCalc << " transforms" << std::endl;
                step *= 10;    
            }
        #endif

        glm::mat4 translationM = glm::translate(position);
        glm::mat4 rotationM = glm::toMat4(rotation);
        glm::mat4 scaleM = glm::scale(scale);

        return translationM * (rotationM * scaleM);
    }


    std::shared_ptr<Component> Transform::loadTransform(Entity &parent, json &entData) {
        glm::vec3 position(0.0f, 0.0f, 0.0f);
        if(entData.contains("position")) {
            json posData = entData["position"];
            position = glm::vec3(posData[0].get<float>(), posData[1].get<float>(), posData[2].get<float>());
        }

        glm::quat rotation(0.0f, 0.0f, 0.0f, 1.0f);
        if(entData.contains("rotation")) {
            json rotData = entData["rotation"];
            rotation = glm::quat(rotData[3].get<float>(), rotData[0].get<float>(), rotData[1].get<float>(), rotData[2].get<float>());
        }

        glm::vec3 scale(1.0f, 1.0f, 1.0f);
        if(entData.contains("scale")) {
            json sclData = entData["scale"];
            scale = glm::vec3(sclData[0].get<float>(), sclData[1].get<float>(), sclData[2].get<float>());
        }

        return std::make_shared<Transform>(parent, position, rotation, scale);
    }

    void Transform::saveComponent(json &entData) {
        entData["position"] = {position[0], position[1], position[2]};

        entData["rotation"] = {rotation.x, rotation.y, rotation.z, rotation.w};

        entData["scale"] = {scale[0], scale[1], scale[2]};
    }
}