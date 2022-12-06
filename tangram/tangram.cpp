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

#include "tengine/mgl.hpp"
#include <tengine/Shape.hpp>
#include <tengine/Color.hpp>
#include <tengine/Point.hpp>

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

  std::vector<Shape> shapes; 

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
    {0.25f, 0.25f, 0.0f, 1.0f},
    {0.75f, 0.25f, 0.0f, 1.0f},
    {0.25f, 0.75f, 0.0f, 1.0f},
    {0.75f, 0.75f, 0.0f, 1.0f}};


const GLubyte Indices[] = {0, 1, 2, 1, 3, 2};

void MyApp::createBufferObjects() {
  glGenVertexArrays(1, &VaoId);
  glBindVertexArray(VaoId);
  {
    glGenBuffers(2, VboId);

    glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
    {
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(POSITION);
        glVertexAttribPointer(POSITION, 4, GL_FLOAT, GL_FALSE, sizeof(tengine::Point), 0);
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboId[1]);
    {
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices,
                   GL_STATIC_DRAW);
    }
  }
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDeleteBuffers(2, VboId);
}

void MyApp::destroyBufferObjects() {
  glBindVertexArray(VaoId);
  glDisableVertexAttribArray(POSITION);
  glDisableVertexAttribArray(COLOR);
  glDeleteVertexArrays(1, &VaoId);
  glBindVertexArray(0);
}

////////////////////////////////////////////////////////////////////////// SCENE

const glm::mat4 I(1.0f);
const glm::mat4 M = glm::translate(glm::vec3(-1.0f, -1.0f, 0.0f));
const glm::mat4 M1 = glm::translate(glm::vec3(-1.0f, 0.0f, 0.0f));

void MyApp::drawScene() {
  // Drawing directly in clip space

  glBindVertexArray(VaoId);
  Shaders->bind();

  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(I));
  glUniform4f(ColorId, 1.0f, 0.0f, 0.0f, 0.0f);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid *)0);

  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(M));
  glUniform4f(ColorId, 0.0f, 1.0f, 0.0f, 0.0f);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid *)0);

  glUniformMatrix4fv(MatrixId, 1, GL_FALSE, glm::value_ptr(M1));
  glUniform4f(ColorId, 0.0f, 0.0f, 1.0f, 0.0f);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, (GLvoid *)0);


  Shaders->unbind();
  glBindVertexArray(0);
}

////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::initCallback(GLFWwindow *win) {
  createBufferObjects();
  createShaderProgram();
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
