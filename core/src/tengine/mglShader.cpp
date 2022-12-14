#include <cassert>
#include <fstream>
#include <memory>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <tengine/mglConventions.hpp>
#include <tengine/mglShader.hpp>
#include <tengine/mglMesh.hpp>

namespace mgl
{

    ////////////////////////////////////////////////////////////////// ShaderProgram

    const std::string ShaderProgram::read(const std::string &filename)
    {
        std::string line, shader_string;
        std::ifstream ifile(filename);
        while (std::getline(ifile, line))
        {
            shader_string += line + "\n";
        }
        return shader_string;
    }

    const GLuint ShaderProgram::checkCompilation(const GLuint shader_id,
                                                 const std::string &filename)
    {
        GLint compiled;
        glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compiled);
        if (compiled == GL_FALSE)
        {
            GLint length;
            glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);
            GLchar *const log = new char[length];
            glGetShaderInfoLog(shader_id, length, &length, log);
            std::cerr << "[" << filename << "] " << std::endl
                      << log;
            delete[] log;
        }
        return compiled;
    }

    void ShaderProgram::checkLinkage()
    {
        GLint linked;
        glGetProgramiv(ProgramId, GL_LINK_STATUS, &linked);
        if (linked == GL_FALSE)
        {
            GLint length;
            glGetProgramiv(ProgramId, GL_INFO_LOG_LENGTH, &length);
            GLchar *const log = new char[length];
            glGetProgramInfoLog(ProgramId, length, &length, log);
            std::cerr << "[LINK] " << std::endl
                      << log << std::endl;
            delete[] log;
            assert(0);
        }
    }

    ShaderProgram::ShaderProgram() : ProgramId(glCreateProgram()) {}

    ShaderProgram::~ShaderProgram()
    {
        glUseProgram(0);
        glDeleteProgram(ProgramId);
    }

    void ShaderProgram::addShader(const GLenum shader_type,
                                  const std::string &filename)
    {
        const GLuint shader_id = glCreateShader(shader_type);
        const std::string scode = read(filename);
        const GLchar *code = scode.c_str();
        glShaderSource(shader_id, 1, &code, 0);
        glCompileShader(shader_id);
        checkCompilation(shader_id, filename);
        glAttachShader(ProgramId, shader_id);

        Shaders[shader_type] = {shader_id};
    }

    void ShaderProgram::addAttribute(const std::string &name, const GLuint index)
    {
        glBindAttribLocation(ProgramId, index, name.c_str());
        Attributes[name] = {index};
    }

    bool ShaderProgram::isAttribute(const std::string &name)
    {
        return Attributes.find(name) != Attributes.end();
    }

    void ShaderProgram::addUniform(const std::string &name)
    {
        Uniforms[name] = {-1};
    }

    bool ShaderProgram::isUniform(const std::string &name)
    {
        return Uniforms.find(name) != Uniforms.end();
    }

    void ShaderProgram::addUniformBlock(const std::string &name,
                                        const GLuint binding_point)
    {
        Ubos[name] = {0, binding_point};
    }

    bool ShaderProgram::isUniformBlock(const std::string &name)
    {
        return Ubos.find(name) != Ubos.end();
    }

    void ShaderProgram::create()
    {
        glLinkProgram(ProgramId);
        checkLinkage();
        for (auto &i : Shaders)
        {
            glDetachShader(ProgramId, i.second);
            glDeleteShader(i.second);
        }

        for (auto &i : Uniforms)
        {
            i.second.index = glGetUniformLocation(ProgramId, i.first.c_str());
            if (i.second.index < 0)
                std::cerr << "WARNING: Uniform " << i.first << " not found." << std::endl;
        }
        for (auto &i : Ubos)
        {
            i.second.index = glGetUniformBlockIndex(ProgramId, i.first.c_str());
            if (i.second.index < 0)
                std::cerr << "WARNING: UBO " << i.first << " not found." << std::endl;
            glUniformBlockBinding(ProgramId, i.second.index, i.second.binding_point);
        }
    }

    void ShaderProgram::bind() { glUseProgram(ProgramId); }

    void ShaderProgram::unbind() { glUseProgram(0); }

    ////////////////////////////////////////////////////////////////////////////////

    std::shared_ptr<ShaderProgram> ShaderProgram::load(std::string &name)
    {
        std::shared_ptr<ShaderProgram> shader = std::make_shared<ShaderProgram>();

        std::string shaderDir = "assets/shaders/";

        std::ifstream shaderFile(shaderDir + name + ".json");
        json shaderData = json::parse(shaderFile);

        std::string fsName = shaderDir + shaderData["fragment"].get<std::string>();
        std::string vsName = shaderDir + shaderData["vertex"].get<std::string>();

        shader->addShader(GL_FRAGMENT_SHADER, fsName);
        shader->addShader(GL_VERTEX_SHADER, vsName);

        shader->addAttribute(mgl::POSITION_ATTRIBUTE, mgl::Mesh::POSITION);
        shader->addAttribute(mgl::NORMAL_ATTRIBUTE, mgl::Mesh::NORMAL);
        shader->addAttribute(mgl::TEXCOORD_ATTRIBUTE, mgl::Mesh::TEXCOORD);
        shader->addAttribute(mgl::TANGENT_ATTRIBUTE, mgl::Mesh::TANGENT);

        shader->addUniform(mgl::COLOR_ATTRIBUTE);
        shader->addUniform(mgl::MODEL_MATRIX);
        shader->addUniformBlock(mgl::CAMERA_BLOCK, mgl::CAMERA_BLOCK_BINDING_POINT);

        shader->create();

        return shader;
    }
} // namespace engine
