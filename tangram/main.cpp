
#include <iostream>
#include <ctime>
#include <filesystem>
#include <vector>
#include <algorithm>

#include <nlohmann/json.hpp>

#include <GL/glew.h>

#include <tengine/mglApp.hpp>
#include <tengine/mglConventions.hpp>
#include <tengine/Resources.hpp>
#include <tengine/Entity.hpp>
#include <tengine/Transform.hpp>
#include <tengine/Component.hpp>
#include <tengine/Input.hpp>

#include "tangram.hpp"
#include "camera.hpp"
#include "door.hpp"
#include "save.hpp"
#include "common.hpp"

////////////////////////////////////////////////////////////////////////// MYAPP
class MyApp : public mgl::App
{
public:
	void initCallback(GLFWwindow *win) override;
	void windowCloseCallback(GLFWwindow *win) override;
	void windowSizeCallback(GLFWwindow *win, int width, int height) override;
};

////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::initCallback(GLFWwindow *win)
{
    tengine::Component::registerComponent("cameraScript", &script::CameraScript::loadScript);
    tengine::Component::registerComponent("saveScript", &script::SaveScript::loadScript);
    tengine::Component::registerComponent("commonScript", &script::CommonScript::loadScript);


	tengine::Component::registerComponent("tangramScript", &tangram::TangramScript::loadScript);
	tengine::Component::registerComponent("doorScript", &door::DoorScript::loadScript);
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
	mgl::Engine::getInstance().getScene()->getCamera().reComputeProjection();
}
/////////////////////////////////////////////////////////////////////////// MAIN

int main(int argc, char *argv[])
{
	mgl::Engine &engine = mgl::Engine::getInstance();
	engine.setApp(new MyApp());
	engine.setOpenGL(4, 6);
	engine.setWindow(600, 600, "3D Tangram:GUN || Group 1 [ASS 3]", 0, 1);
	engine.init("assets/scenes/door.json");
	engine.run();
	exit(EXIT_SUCCESS);
}

//////////////////////////////////////////////////////////////////////////// END