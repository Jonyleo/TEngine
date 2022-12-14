#ifndef __RENDERER_HPP__
#define __RENDERER_HPP__

#include <memory>

#include <glm/glm.hpp>

#include "mglShader.hpp"

namespace tengine
{
    class Renderer
    {
    private:
        std::shared_ptr<mgl::ShaderProgram> currentShader;

        Renderer() {}

    protected:
        ~Renderer() {}
        static Renderer &getInstance();

    public:
        static void setMat4(const std::string &name, const glm::mat4 &value);
        static void setVec4(const std::string &name, const glm::vec4 &value);

        static void setCurrentShader(std::shared_ptr<mgl::ShaderProgram> shader);
        Renderer(Renderer const &) = delete;
        void operator=(Renderer const &) = delete;
    };
}

#endif