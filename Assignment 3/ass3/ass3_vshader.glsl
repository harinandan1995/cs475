#version 330

in vec4 vPosition;
in vec4 vColor;
in vec3 vNormal;
in vec2 vTex;

varying vec2 tex;
out vec4 color;
uniform mat4 uModelViewMatrix;

void main (void) 
{
  gl_Position = uModelViewMatrix * vPosition;
  color = vColor;
  tex = vTex;
}
