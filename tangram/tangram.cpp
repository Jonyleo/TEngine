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

#include <GL/glew.h>

#include <tengine/mglApp.hpp>
#include <tengine/Resources.hpp>
#include <tengine/Entity.hpp>
#include <tengine/Transform.hpp>
#include <tengine/Component.hpp>

////////////////////////////////////////////////////////////////////////// MYAPP

class MyApp : public mgl::App
{

public:
	void initCallback(GLFWwindow *win) override;
	void windowCloseCallback(GLFWwindow *win) override;
	void windowSizeCallback(GLFWwindow *win, int width, int height) override;
	void keyCallback(GLFWwindow *window, int key, int scancode,
					 int action, int mods);
	void scrollCallback(GLFWwindow *window, double xoffset,
						double yoffset);
};

// Eye(5,5,5) Center(0,0,0) Up(0,1,0)
const glm::mat4 ViewMatrix1 =
	glm::lookAt(glm::vec3(0, 0, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, 1.0f, 0.0f));

// Eye(-5,-5,-5) Center(0,0,0) Up(0,1,0)
const glm::mat4 ViewMatrix2 =
	glm::lookAt(glm::vec3(-5.0f, -5.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f),
				glm::vec3(0.0f, 1.0f, 0.0f));

// Orthographic LeftRight(-2,2) BottomTop(-2,2) NearFar(1,10)
const glm::mat4 ProjectionMatrix1 =
	glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, 1.0f, 10.0f);

// Perspective Fovy(30) Aspect(640/480) NearZ(1) FarZ(10)
const glm::mat4 ProjectionMatrix2 =
	glm::perspective(glm::radians(30.0f), 640.0f / 480.0f, 1.0f, 10.0f);

int transition = 0;

int directionX = 0;
int directionY = 0;
int rotateZ = 0;
int rotateY = 0;
double scale = 0;

class TangramScript : public tengine::Component
{
	struct Transition
	{
		glm::vec3 position;
		glm::quat rotation;
	};

	float speed = 0;
	float transitionTime = 0.5; // transition takes 5 seconds
	float timePassed = 0;
	std::map<std::string, std::shared_ptr<tengine::Transform>> pieceTransforms;
	std::map<std::string, std::shared_ptr<tengine::Transform>> boxTransforms;
	std::map<std::string, Transition> transitionMap;
	std::map<std::string, Transition> initialMap;

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

			transitionMap[childName] = {boxTrans->getPosition(), boxTrans->getRotation()};
			initialMap[childName] = {childTrans->getPosition(), childTrans->getRotation()};
		}
	}

	void update(double elapsedTime)
	{
		std::shared_ptr<tengine::Transform> trans = entity.getComponent<tengine::Transform>();

		glm::vec3 rotation = {0.0f, 360.0f * elapsedTime * rotateY, 360.0f * elapsedTime * rotateZ};
		trans->rotateBy(tengine::Transform::quaternionFromAxis(rotation));
		;
		trans->moveBy(glm::vec3(3 * elapsedTime * directionX, 3 * elapsedTime * directionY, 0));

		double scaleL = 1 + 0.3 * scale;
		trans->scaleBy({scaleL, scaleL, scaleL});

		scale = 0;

		if (transition == 0)
			return;

		timePassed += elapsedTime * transition;
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

	TangramScript(tengine::Entity &entity) : Component(entity) {}
};

////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::initCallback(GLFWwindow *win)
{
	tengine::Scene &scene = mgl::Engine::getInstance().getScene();
	scene.getRoot().attachComponent(std::make_shared<TangramScript>(scene.getRoot()));
	scene.getCamera().setViewMatrix(ViewMatrix1);
	scene.getCamera().setProjectionMatrix(ProjectionMatrix2);
	scene.getRoot().init();
}

void MyApp::windowCloseCallback(GLFWwindow *win) { tengine::ResourceManager::getInstance().clear(); }

void MyApp::windowSizeCallback(GLFWwindow *win, int winx, int winy)
{
	glViewport(0, 0, winx, winy);
	mgl::Engine::getInstance().WindowHeight = winy;
	mgl::Engine::getInstance().WindowWidth = winx;
	mgl::Engine::getInstance().getScene().getCamera().setAspectRatio(winx / (float)winy);
}

void MyApp::keyCallback(GLFWwindow *window, int key, int scancode,
						int action, int mods)
{

	if (action == GLFW_PRESS)
	{
		switch (key)
		{
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

		case GLFW_KEY_E:
			rotateZ -= 1;
			break;
		case GLFW_KEY_Q:
			rotateZ += 1;
			break;

		case GLFW_KEY_Z:
			rotateY -= 1;
			break;
		case GLFW_KEY_C:
			rotateY += 1;
			break;

		case GLFW_KEY_ESCAPE:
			mgl::Engine::getInstance().close();
			break;

		case GLFW_KEY_LEFT:
			transition += 1;
			break;

		case GLFW_KEY_RIGHT:
			transition -= 1;
			break;
		}
	}
	if (action == GLFW_RELEASE)
	{
		switch (key)
		{
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

		case GLFW_KEY_E:
			rotateZ += 1;
			break;
		case GLFW_KEY_Q:
			rotateZ -= 1;
			break;

		case GLFW_KEY_Z:
			rotateY += 1;
			break;
		case GLFW_KEY_C:
			rotateY -= 1;
			break;

		case GLFW_KEY_LEFT:
			transition -= 1;
			break;

		case GLFW_KEY_RIGHT:
			transition += 1;
			break;
		}
	}
}

void MyApp::scrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
	scale += yoffset;
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
