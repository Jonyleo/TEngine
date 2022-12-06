#ifndef __TRANSFORM_HPP__
#define __TRANSFORM_HPP__

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "Point.hpp"


namespace tengine {
    class Transform {
        private:
            float rotation; // radians
            float scale; 
            glm::vec2 position; // clipSpace

            // transformMatrix Cache
            bool changed = true;
            glm::mat4 transformMatrix;

        public:
            Transform() : Transform({0,0}, 0, 1) {}
            Transform(glm::vec2 position, float rotation, float scale)
                : position(position), rotation(rotation), scale(scale) {}

            void rotateTo(float angle) { rotation = angle; changed = true; }
            void rotateBy(float angle) { rotation += angle; changed = true; }
            void moveTo(glm::vec2 destination) {position = destination; changed = true; }
            void moveBy(glm::vec2 direction) {position += direction; changed = true; }
            void scaleTo(float scale) { scale = scale; changed = true; }
            void scaleBy(float factor) { scale *= factor; changed = true; }

            glm::mat4& calcTransformMatrix() {
                if (!changed)
                    return transformMatrix;
                    
                glm::mat4 rotationM = glm::rotate(rotation, glm::vec3(0.0f, 0.0f, 1.0f));
                glm::mat4 scaleM = glm::scale(glm::vec3(scale, scale, 1.0f));
                glm::mat4 translationM = glm::translate(glm::vec3(position, 0.0f));
                transformMatrix = translationM * (scaleM * rotationM);

                changed = false;

                return transformMatrix;
            }
    };
}
#endif