////////////////////////////////////////////////////////////////////////////////
//	CGJ Project - 2022-2023
//	
//	"Assignment 3 - 3D TANGRAM"
//
//	Group 1:
//		Joao Oliveira  | 93728
//		Giulio Camasso | 105147
////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include <GL/glew.h>

#include <tengine/mglApp.hpp>
#include <tengine/mglConventions.hpp>
#include <tengine/Resources.hpp>
#include <tengine/Entity.hpp>
#include <tengine/Transform.hpp>
#include <tengine/Component.hpp>

////////////////////////////////////////////////////////////////////////// MYAPP
class MyApp : public mgl::App
{
friend class TangramScript;

private:

	std::shared_ptr<mgl::CameraBuffer> cameraBuff;
	std::vector<std::shared_ptr<mgl::Camera>> cameras;
	
	int nCamera = 0;
	int transition = 0;
	bool rotate = false;

	double mouseX = 0;
	double mouseY = 0;
	double lastX = 0;
	double lastY = 0;

	double zoom = 0;

	bool switchCamera = false;
	bool switchPerspective = true;

public:
	void initCallback(GLFWwindow *win) override;
	void windowCloseCallback(GLFWwindow *win) override;
	void windowSizeCallback(GLFWwindow *win, int width, int height) override;
	void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
	void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
	void cursorCallback(GLFWwindow *window, double xpos, double ypos);
	void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
};

class TangramScript : public tengine::Component
{
	struct Transition
	{
		glm::vec3 position;
		glm::quat rotation;
	};

private:

	MyApp& app;
	float transitionTime = 0.5;
	float timePassed = 0;

	std::map<std::string, std::shared_ptr<tengine::Transform>> pieceTransforms;
	std::map<std::string, std::shared_ptr<tengine::Transform>> boxTransforms;

	std::map<std::string, Transition> transitionMap;
	std::map<std::string, Transition> initialMap;

	float sensitivity = 1.5;

public:
	void init()
	{
		for (auto &child : entity.getChildren())
		{
			if (!child->getName().ends_with("Box"))
				pieceTransforms[child->getName()] = child->getComponent<tengine::Transform>();
			else
				boxTransforms[child->getName()] = child->getComponent<tengine::Transform>();
		}

		for (auto &box : boxTransforms)
		{
			std::shared_ptr<tengine::Transform> boxTrans = box.second;
			std::string childName = box.first.substr(0, box.first.length() - 3);
			std::shared_ptr<tengine::Transform> childTrans = pieceTransforms[childName];

			transitionMap[childName] = {
				boxTrans->getPosition(),
				boxTrans->getRotation()
			};

			initialMap[childName] = {
				childTrans->getPosition(),
				childTrans->getRotation()
			};
		}
	}

	void update(double elapsedTime)
	{
		if(app.switchCamera) {
			app.nCamera = (static_cast<unsigned long long>(app.nCamera) + 1) % app.cameras.size();
			mgl::Engine::getInstance().getScene().setCamera(app.cameras[app.nCamera]);
			app.switchCamera = false;
		}

		if(app.zoom != 0) {
			mgl::Engine::getInstance().getScene().getCamera().zoomBy(app.zoom);
			app.zoom = 0;
		}

		if(app.rotate) {
			float movementX = -(app.mouseX - app.lastX) * sensitivity; 
			float movementY = (app.mouseY - app.lastY) * sensitivity;

			mgl::Engine::getInstance().getScene().getCamera().rotate(movementX, movementY);

			app.lastX = app.mouseX;
			app.lastY = app.mouseY;
		}

		if(app.switchPerspective) {
			mgl::Engine::getInstance().getScene().getCamera().switchPerspective();
			app.switchPerspective = false;
		}

		if (app.transition == 0)
			return;

		timePassed += elapsedTime * app.transition;
		if (timePassed > transitionTime)
			timePassed = transitionTime;
		if (timePassed < 0)
			timePassed = 0;

		for (auto &transitionPair : transitionMap)
		{
			std::shared_ptr<tengine::Transform> childTrans = pieceTransforms[transitionPair.first];

			glm::quat deltaRotation = glm::mix(
				initialMap[transitionPair.first].rotation,
				transitionMap[transitionPair.first].rotation,
				timePassed / transitionTime);

			glm::vec3 deltaPosition = glm::mix(
				initialMap[transitionPair.first].position,
				transitionMap[transitionPair.first].position,
				timePassed / transitionTime);

			// childTrans->rotateBy(tengine::Transform::quaternionFromAxis({0.0f, 90.0f * elapsedTime, 0.0f}));
			childTrans->rotateTo(deltaRotation);
			childTrans->moveTo(deltaPosition);
		}
	}

	TangramScript(tengine::Entity &entity) : Component(entity), app(*(MyApp *)mgl::Engine::getInstance().getApp()) {}
};

////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::initCallback(GLFWwindow *win)
{
	cameraBuff = std::make_shared<mgl::CameraBuffer>(mgl::CAMERA_BLOCK_BINDING_POINT);

	// camera 0 (def: tangram figure composed)
	cameras.push_back(
		std::make_shared<mgl::Camera>(
			cameraBuff, 
			5.0f,
			glm::radians(mgl::FOV),
			mgl::zNear,
			mgl::zFar
		)
	);
	cameras[0]->rotate(glm::radians(90.0), 0);

	// camera 1 (def: tangram box configuration)
	cameras.push_back(
		std::make_shared<mgl::Camera>(
			cameraBuff, 
			5.0f,
			glm::radians(mgl::FOV),
			mgl::zNear,
			mgl::zFar
		)
	);
	
	tengine::Scene &scene = mgl::Engine::getInstance().getScene();

	nCamera = 0;
	scene.setCamera(cameras[nCamera]);

	scene.getRoot().attachComponent(std::make_shared<TangramScript>(scene.getRoot()));

	scene.getRoot().init();
}

void MyApp::windowCloseCallback(GLFWwindow *win)
{
	tengine::ResourceManager::getInstance().clear();
}

void MyApp::windowSizeCallback(GLFWwindow *win, int winx, int winy)
{
	glViewport(0, 0, winx, winy);
	mgl::Engine::getInstance().WindowHeight = winy;
	mgl::Engine::getInstance().WindowWidth = winx;
	mgl::Engine::getInstance().getScene().getCamera().reComputeProjection();
}

void MyApp::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{

	if (action == GLFW_PRESS)
	{
		switch (key) {
			case GLFW_KEY_ESCAPE:
				mgl::Engine::getInstance().close();
				break;

			case GLFW_KEY_LEFT:
				transition += 1;
				break;

			case GLFW_KEY_RIGHT:
				transition -= 1;
				break;

			case GLFW_KEY_P:
				switchPerspective = true;
				break;

			case GLFW_KEY_C:
				switchCamera = true;
				break;
		}
	}

	if(action == GLFW_RELEASE) {
		switch (key)
		{
		case GLFW_KEY_LEFT:
			transition -= 1;
			break;

		case GLFW_KEY_RIGHT:
			transition += 1;
			break;
		}
	}
}

void MyApp::cursorCallback(GLFWwindow *window, double xpos, double ypos) {
	mouseX = xpos / mgl::Engine::getInstance().WindowWidth;
	mouseY = ypos / mgl::Engine::getInstance().WindowHeight;
}

void MyApp::scrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
	zoom += yoffset;
}

void MyApp::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
	if(action == GLFW_PRESS) {
		switch(button) {
			case GLFW_MOUSE_BUTTON_1:
				rotate = true;
				lastX = mouseX;
				lastY = mouseY;
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				break;
		}
	} 
	if(action == GLFW_RELEASE) {
		switch(button) {
			case GLFW_MOUSE_BUTTON_1:
				rotate = false;
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
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
	engine.setWindow(600, 600, "3D Tangram:GUN || Group 1 [ASS 3]", 0, 1);
	engine.init("tangram");
	engine.run();
	exit(EXIT_SUCCESS);
}

//////////////////////////////////////////////////////////////////////////// END
