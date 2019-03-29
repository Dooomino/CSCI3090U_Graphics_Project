#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 texture;


uniform mat4 MVP;
uniform mat4 model;

out vec3 normal;
out vec3 FragPos;
out vec2 textureCoord;
out vec3 outPosition;

void main(){
  gl_Position = MVP * vec4(position,1.0);
  outPosition = position;
  FragPos = vec3(model * vec4(position, 1.0));
  normal = vec3(MVP*vec4(aNormal,1.0));
  textureCoord = texture;
}