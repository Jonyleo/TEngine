#version 330 core

in vec4 inPosition;
out vec4 exColor;

uniform mat4 Matrix;
uniform vec4 inColor;

void main(void) {
  gl_Position = Matrix * inPosition;
  exColor = (inColor);
}
