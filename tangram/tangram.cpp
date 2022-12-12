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
#include <tengine/Resources.hpp>


#include <tengine/json.hpp>

using json = nlohmann::json;

////////////////////////////////////////////////////////////////////////// MYAPP

class MyApp : public mgl::App
{

public:
	void initCallback(GLFWwindow *win) override;
	void windowCloseCallback(GLFWwindow *win) override;
	void windowSizeCallback(GLFWwindow *win, int width, int height) override;
	void keyCallback(GLFWwindow *window, int key, int scancode,
                                 int action, int mods);
};

int directionX = 0;
int directionY = 0;
int rotate = 0;
int scale = 0;

class TangramScript : public tengine::Component
{
	float speed = 0;

	public:
	void update(float elapsedTime) {
		std::shared_ptr<tengine::Transform> trans = parent.getComponent<tengine::Transform>();
		
		trans->rotateBy(glm::radians(360.0f) * elapsedTime * rotate);
		glm::vec2 move(3 * elapsedTime*directionX, 3*elapsedTime*directionY);
		trans->moveBy(move);
		trans->scaleBy(1 + 0.1 * scale);

	}

	TangramScript(tengine::Entity &entity) : Component(entity) {}
};

////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::initCallback(GLFWwindow *win)
{
	tengine::Scene& scene = mgl::Engine::getInstance().getScene();
	scene.getRoot().attachComponent(std::make_shared<TangramScript>(scene.getRoot()));
}

void MyApp::windowCloseCallback(GLFWwindow *win) { tengine::ResourceManager::getInstance().clear(); }

void MyApp::windowSizeCallback(GLFWwindow *win, int winx, int winy)
{
	glViewport(0, 0, winx, winy);
	mgl::Engine::getInstance().WindowHeight = winy;
	mgl::Engine::getInstance().WindowWidth = winx;
}

void MyApp::keyCallback(GLFWwindow *window, int key, int scancode,
                                 int action, int mods) { 

	if(action == GLFW_PRESS) {
		switch(key) {
			case GLFW_KEY_W:
				directionY += 1;
				break;
			case GLFW_KEY_S:
				directionY -= 1;
				break;
			case GLFW_KEY_A:
				directionX -= 1;
				break;
			case GLFW_KEY_D:
				directionX += 1;
				break;
			
			case GLFW_KEY_SPACE:
				rotate = 1;
				break;
			case GLFW_KEY_Q:
				scale -= 1;
				break;
			case GLFW_KEY_E:
				scale += 1;
				break;
		}
	} 
	if(action == GLFW_RELEASE) {
		switch(key) {
			case GLFW_KEY_W:
				directionY -= 1;
				break;
			case GLFW_KEY_S:
				directionY += 1;
				break;
			case GLFW_KEY_A:
				directionX += 1;
				break;
			case GLFW_KEY_D:
				directionX -= 1;
				break;
			
			case GLFW_KEY_SPACE:
				rotate = 0;
				break;
			case GLFW_KEY_Q:
				scale += 1;
				break;
			case GLFW_KEY_E:
				scale -= 1;
				break;
		}
	}
		
}

/////////////////////////////////////////////////////////////////////////// MAIN

int main(int argc, char *argv[])
{
	mgl::Engine &engine = mgl::Engine::getInstance();
	engine.setApp(new MyApp());
	engine.setOpenGL(4, 6);
	engine.setWindow(600, 600, "Tangram || Group 1 [ASS 2]", 0, 1);
	engine.init("tangram");
	engine.run();
	exit(EXIT_SUCCESS);
}

//////////////////////////////////////////////////////////////////////////// END
