#ifndef __TANGRAM_HPP__
#define __TANGRAM_HPP__

#include <iostream>
#include <ctime>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <memory>

#include <nlohmann/json.hpp>

#include <GL/glew.h>

#include <tengine/mglCamera.hpp>
#include <tengine/Resources.hpp>
#include <tengine/Entity.hpp>
#include <tengine/Transform.hpp>
#include <tengine/Component.hpp>
#include <tengine/Input.hpp>

namespace tangram {
    using json = nlohmann::json;

    class TangramScript : public tengine::Component
    {
        struct Transition
        {
            glm::vec3 position;
            glm::quat rotation;
        };

    private:

        const float transitionTime = 0.5;

        float timePassed = 0;
        int transition = 0;

        std::map<std::string, std::shared_ptr<tengine::Transform>> startTransforms;
        std::map<std::string, std::shared_ptr<tengine::Transform>> endTransforms;
        std::map<std::string, std::shared_ptr<tengine::Transform>> pieceTransforms;


        void handleLoadFile(int key);

    public:	
        void init();
        void update(double elapsedTime);

        std::string getId() { return "tangramScript"; }
        
        TangramScript(tengine::Entity &entity);
        TangramScript(tengine::Entity &entity, json &data);

        void saveComponent(json &data);
        

        static std::shared_ptr<Component> loadScript(tengine::Entity &entity, json &data) {
            return std::make_shared<TangramScript>(entity, data);
        };
    };
}

#endif