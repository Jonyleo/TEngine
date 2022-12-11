////////////////////////////////////////////////////////////////////////////////
//
// Drawing two instances of a triangle in Clip Space.
// A "Hello 2D World" of Modern OpenGL.
//
// (c)2013-22 by Carlos Martinho
//
// INTRODUCES:
// GL PIPELINE, mglShader.hpp, mglConventions.hpp
//
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <fstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <tengine/tengine.hpp>

#include <tengine/json.hpp>

using json = nlohmann::json;

////////////////////////////////////////////////////////////////////////// MYAPP

class MyApp : public mgl::App
{

public:
	void initCallback(GLFWwindow *win) override;
	void displayCallback(GLFWwindow *win, double elapsed) override;
	void windowCloseCallback(GLFWwindow *win) override;
	void windowSizeCallback(GLFWwindow *win, int width, int height) override;

private:
	const GLuint POSITION = 0;
	std::shared_ptr<mgl::ShaderProgram> Shaders;

	void createShaderProgram();
	void createEntities();
	void destroyBufferObjects();
	void drawScene();
};

//////////////////////////////////////////////////////////////////////// SHADERs

void MyApp::createShaderProgram()
{

	Shaders = std::make_shared<mgl::ShaderProgram>();

	Shaders->addShader(GL_VERTEX_SHADER, "clip-vs.glsl");
	Shaders->addShader(GL_FRAGMENT_SHADER, "clip-fs.glsl");

	Shaders->addAttribute(mgl::POSITION_ATTRIBUTE, POSITION);
	Shaders->addUniform(mgl::MODEL_MATRIX);
	Shaders->addUniform(mgl::COLOR_ATTRIBUTE);

	Shaders->create();
}

//////////////////////////////////////////////////////////////////// VAOs & VBOs

std::map<std::string, std::shared_ptr<tengine::Mesh>> meshes;
std::map<std::string, std::shared_ptr<tengine::Entity>> entities;

void MyApp::createEntities()
{
	tengine::VertexAttrInfo positionAttr = {POSITION, 4, GL_FLOAT, GL_FALSE, sizeof(tengine::Point), 0};

	std::ifstream meshFile("assets/objects/meshes.json");
	json meshData = json::parse(meshFile);

	for (json::iterator it = meshData.begin(); it != meshData.end(); ++it)
	{
		std::string id = it.key();
		std::cout << "[LOADING MESH] " << id << std::endl;

		// Load vertex data
		json vertexData = it.value()["vertex"];
		size_t n_vertex = vertexData.size();
		std::vector<tengine::Point> vertexes(n_vertex);
		for (int i = 0; i < n_vertex; ++i)
		{
			vertexes[i] = {vertexData[i][0].get<float>(), vertexData[i][1].get<float>(), 0.0f, 1.0f};
		}

		// Load index data
		json indexData = it.value()["index"];
		size_t n_index = indexData.size();
		std::vector<GLubyte> indexes(n_index);
		for (int i = 0; i < n_index; ++i)
		{
			indexes[i] = indexData[i].get<GLuint>();
		}

		// Create the mesh
		meshes[id] = std::make_shared<tengine::Mesh>(n_index);
		meshes[id]->createVertexBuffer(vertexes.data(), vertexes.size() * sizeof(tengine::Point));
		meshes[id]->createIndexBuffer(indexes.data(), indexes.size() * sizeof(GLubyte));
		meshes[id]->createArrayBuffer(&positionAttr, 1);
	}

	std::ifstream entityFile("assets/objects/entities.json");
	json entityData = json::parse(entityFile);

	for (json::iterator it = entityData.begin(); it != entityData.end(); ++it)
	{
		std::string id = it.key();
		std::cout << "[LOADING ENTITY] " << id << std::endl;

		json data = it.value();

		// Get the previously created mesh
		std::shared_ptr<tengine::Mesh> mesh = meshes[data["mesh"].get<std::string>()];

		// Get position, rotation and scale
		json posData = data["position"];
		glm::vec2 position(posData[0].get<float>(), posData[1].get<float>());
		float rotation = glm::radians(data["rotation"].get<float>());
		float scale = data["scale"].get<float>();

		// Get color
		json colorData = data["color"];
		tengine::Color color = {0};
		for (int i = 0; i < 4; ++i)
		{
			color.RGBA[i] = colorData[i].get<int>() / 255.0f;
		}

		// Create the entity
		entities[id] = std::make_shared<tengine::Entity>();
		entities[id]->attachComponent(std::make_shared<tengine::MeshInstance>(color, Shaders, mesh));
		entities[id]->attachComponent(std::make_shared<tengine::Transform>(position, rotation, scale));
	}
}

void MyApp::destroyBufferObjects()
{
	// Destructors take care of destroying buffer objects
	entities.clear();
}

////////////////////////////////////////////////////////////////////////// SCENE

void MyApp::drawScene()
{
	for (auto &entity : entities)
	{
		entity.second->draw();
	}
}

////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::initCallback(GLFWwindow *win)
{
	createShaderProgram();
	createEntities();
}

void MyApp::windowCloseCallback(GLFWwindow *win) { destroyBufferObjects(); }

void MyApp::windowSizeCallback(GLFWwindow *win, int winx, int winy)
{
	glViewport(0, 0, winx, winy);
	mgl::Engine::getInstance().WindowHeight = winy;
	mgl::Engine::getInstance().WindowWidth = winx;
}

void MyApp::displayCallback(GLFWwindow *win, double elapsed) { drawScene(); }

/////////////////////////////////////////////////////////////////////////// MAIN

int main(int argc, char *argv[])
{
	mgl::Engine &engine = mgl::Engine::getInstance();
	engine.setApp(new MyApp());
	engine.setOpenGL(4, 6);
	engine.setWindow(1200, 600, "Hello Modern 2D World", 0, 1);
	engine.init();
	engine.run();
	exit(EXIT_SUCCESS);
}

//////////////////////////////////////////////////////////////////////////// END
