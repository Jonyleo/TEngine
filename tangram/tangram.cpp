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

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <tengine/mgl.hpp>
#include <tengine/Color.hpp>
#include <tengine/Point.hpp>
#include <tengine/Mesh.hpp>
#include <tengine/Transform.hpp>
#include <tengine/Entity.hpp>

////////////////////////////////////////////////////////////////////////// MYAPP

class MyApp : public mgl::App {

public:
  void initCallback(GLFWwindow *win) override;
  void displayCallback(GLFWwindow *win, double elapsed) override;
  void windowCloseCallback(GLFWwindow *win) override;
  void windowSizeCallback(GLFWwindow *win, int width, int height) override;

private:
  const GLuint POSITION = 0, COLOR = 1;
  GLuint VaoId, VboId[3];
  mgl::ShaderProgram *Shaders;
  GLint MatrixId;
  GLint ColorId;


  void createShaderProgram();
  void createBufferObjects();
  void destroyBufferObjects();
  void drawScene();
};

//////////////////////////////////////////////////////////////////////// SHADERs

void MyApp::createShaderProgram() {

  Shaders = new mgl::ShaderProgram();

  Shaders->addShader(GL_VERTEX_SHADER, "clip-vs.glsl");
  Shaders->addShader(GL_FRAGMENT_SHADER, "clip-fs.glsl");

  Shaders->addAttribute(mgl::POSITION_ATTRIBUTE, POSITION);
  Shaders->addUniform("Matrix");
  Shaders->addUniform("inColor");

  Shaders->create();

  MatrixId = Shaders->Uniforms["Matrix"].index;
  ColorId = Shaders->Uniforms["inColor"].index;
}

//////////////////////////////////////////////////////////////////// VAOs & VBOs


const tengine::Point Vertices[] = {
    {-0.25f, -0.25f, 0.0f, 1.0f},
    {0.25f, -0.25f, 0.0f, 1.0f},
    {-0.25f, 0.25f, 0.0f, 1.0f},
    {0.25f, 0.25f, 0.0f, 1.0f}};

const GLubyte Indexes[] = {0, 1, 2, 1, 3, 2};

tengine::Mesh *triangleMesh = nullptr;
tengine::Transform transform;

tengine::Entity *redTriangle = nullptr;
tengine::Entity *blueTriangle = nullptr;
tengine::Entity *greenTriangle = nullptr;
tengine::Entity *blackTriangle = nullptr;

void MyApp::createBufferObjects() {
    triangleMesh = new tengine::Mesh(3);
    triangleMesh->createVertexBuffer(Vertices, sizeof(Vertices));
    triangleMesh->createIndexBuffer(Indexes, sizeof(Indexes));
    triangleMesh->createArrayBuffer(POSITION, 4, sizeof(tengine::Point));

    redTriangle = new tengine::Entity(tengine::RED, transform, *triangleMesh, Shaders);
    blueTriangle = new tengine::Entity(tengine::BLUE, transform, *triangleMesh, Shaders);
    greenTriangle = new tengine::Entity(tengine::GREEN, transform, *triangleMesh, Shaders);
    blackTriangle = new tengine::Entity(tengine::BLACK, transform, *triangleMesh, Shaders);
    
    redTriangle->transform.moveTo(glm::vec2(-0.5f, -0.5f));
    
    blueTriangle->transform.moveTo(glm::vec2(0.5f, -0.5f));
    blueTriangle->transform.rotateBy(glm::radians(90.0f));

    greenTriangle->transform.moveTo(glm::vec2(-0.5f, 0.5f));
    greenTriangle->transform.scaleBy(2);

    blackTriangle->transform.moveTo(glm::vec2(0.5f, 0.5f));
    blackTriangle->transform.scaleBy(0.5);
    blackTriangle->transform.rotateBy(glm::radians(-45.0f));

}

void MyApp::destroyBufferObjects() {
  delete triangleMesh;
  delete redTriangle;
  delete blueTriangle;
  delete greenTriangle;
  delete blackTriangle;
}

////////////////////////////////////////////////////////////////////////// SCENE

void MyApp::drawScene() {
    // Drawing directly in clip space
    if(redTriangle != nullptr)
        redTriangle->draw();

    if(blueTriangle != nullptr)
        blueTriangle->draw();

    if(greenTriangle != nullptr)
        greenTriangle->draw();

    if(blackTriangle != nullptr)
        blackTriangle->draw();

    
}

////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::initCallback(GLFWwindow *win) {
  createShaderProgram();
  createBufferObjects();
}

void MyApp::windowCloseCallback(GLFWwindow *win) { destroyBufferObjects(); }

void MyApp::windowSizeCallback(GLFWwindow *win, int winx, int winy) {
  glViewport(0, 0, winx, winy);
}

void MyApp::displayCallback(GLFWwindow *win, double elapsed) { drawScene(); }

/////////////////////////////////////////////////////////////////////////// MAIN

int main(int argc, char *argv[]) {
  mgl::Engine &engine = mgl::Engine::getInstance();
  engine.setApp(new MyApp());
  engine.setOpenGL(4, 6);
  engine.setWindow(600, 600, "Hello Modern 2D World", 0, 1);
  engine.init();
  engine.run();
  exit(EXIT_SUCCESS);
}

//////////////////////////////////////////////////////////////////////////// END
