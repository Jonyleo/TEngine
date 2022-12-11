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
	mgl::ShaderProgram *Shaders;

	void createShaderProgram();
	void createEntities();
	void destroyBufferObjects();
	void drawScene();
};

//////////////////////////////////////////////////////////////////////// SHADERs

void MyApp::createShaderProgram()
{

	Shaders = new mgl::ShaderProgram();

	Shaders->addShader(GL_VERTEX_SHADER, "clip-vs.glsl");
	Shaders->addShader(GL_FRAGMENT_SHADER, "clip-fs.glsl");

	Shaders->addAttribute(mgl::POSITION_ATTRIBUTE, POSITION);
	Shaders->addUniform("Matrix");
	Shaders->addUniform("inColor");

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

	for(json::iterator it = meshData.begin() ; it != meshData.end(); ++it) {
		std::string id = it.key();
		std::cout << "[LOADING MESH] " << id << std::endl;
		json vertexData = it.value()["vertex"];
		size_t n_vertex = vertexData.size();

		// Type * x = static_cast<Type *>(malloc(sizeof(Type))
		tengine::Point *points = (tengine::Point*) (malloc(sizeof(tengine::Point) * n_vertex));
		// tengine::Point *points = static_cast<tengine::Point *> (malloc(sizeof(tengine::Point) * n_vertex));
		// tengine::Point points[n_vertex]({{0, 0, 0, 0}});

		for (int i = 0; i < n_vertex; ++i)
		{
			std::cout << "( " << vertexData[i][0].get<float>() << ", " << vertexData[i][1].get<float>() << ")" << std::endl;
			points[i] = {vertexData[i][0].get<float>(), vertexData[i][1].get<float>(), 0.0f, 1.0f};
		}
		json indexData = it.value()["index"];
		size_t n_index = indexData.size();

		// GLubyte *indexes = malloc(sizeof(GLubyte) * n_index)
		GLubyte *indexes = (GLubyte *) (malloc(sizeof(GLubyte) * n_index));

		// GLubyte indexes[n_index];

		for (int i = 0; i < n_index; ++i)
		{
			std::cout << indexData[i].get<GLubyte>() << std::endl;
			indexes[i] = indexData[i].get<GLubyte>();
		}


		meshes[id] = std::make_shared<tengine::Mesh>(n_index);
		meshes[id]->createVertexBuffer(points, sizeof(points));
		meshes[id]->createIndexBuffer(indexes, sizeof(indexes));
		meshes[id]->createArrayBuffer(&positionAttr, 1);
		}


		std::ifstream entityFile("assets/objects/entities.json");
		json entityData = json::parse(entityFile);

		for (json::iterator it = entityData.begin(); it != entityData.end(); ++it)
		{
			std::string id = it.key();
			std::cout << "[LOADING ENTITY] " << id << std::endl;

			json data = it.value();
			std::shared_ptr<tengine::Mesh> mesh = meshes[data["mesh"].get<std::string>()];

			json posData = data["position"];
			glm::vec2 position(posData[0].get<float>(), posData[1].get<float>());

			float rotation = glm::radians(data["rotation"].get<float>());
			float scale = data["scale"].get<float>();

			std::cout << glm::to_string(position) << std::endl;
			std::cout << rotation << std::endl;
			std::cout << scale << std::endl;

			tengine::Transform transform(position, rotation, scale);

			json colorData = data["color"];

			tengine::Color color = {0};

			for (int i = 0; i < 4; ++i)
			{
				color.RGBA[i] = colorData[i].get<int>() / 255.0f;
				std::cout << color.RGBA[i] << std::endl;
			}

			entities[id] = std::make_shared<tengine::Entity>(color, transform, *mesh, Shaders);
	}
}

void MyApp::destroyBufferObjects()
{
	entities.clear();
}

////////////////////////////////////////////////////////////////////////// SCENE

void MyApp::drawScene()
{
	for(auto& entity : entities) {
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
