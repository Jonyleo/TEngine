#ifndef __SHADERMANAGER_HPP__
#define __SHADERMANAGER_HPP__

#include <memory>

#include <glm/glm.hpp>

#include "mglShader.hpp"

namespace tengine
{
    class ShaderManager
    {
    private:
        std::shared_ptr<mgl::ShaderProgram> currentShader;

        ShaderManager() {}

    protected:
        ~ShaderManager() {}

    public:
        static ShaderManager &getInstance();

        void setMat4(const std::string &name, const glm::mat4 &value);
        void setVec4(const std::string &name, const glm::vec4 &value);

        void setCurrentShader(std::shared_ptr<mgl::ShaderProgram> shader);

        ShaderManager(ShaderManager const &) = delete;
        void operator=(ShaderManager const &) = delete;
    };
}

#endif