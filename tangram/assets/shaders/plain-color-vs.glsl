#version 330 core

in vec4 inPosition;
out vec4 exColor;

uniform mat4 ModelMatrix;
uniform vec4 inColor;

void main(void) {
  gl_Position = ModelMatrix * inPosition;
  exColor = (inColor);
}
