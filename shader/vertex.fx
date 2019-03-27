#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 aNormal;

uniform mat4 MVP;
uniform mat4 model;

out vec3 normal;
out vec3 FragPos;


void main(){
  gl_Position = MVP * vec4(position,1.0);
  FragPos = vec3(model * vec4(position, 1.0));
  normal = vec3(MVP * vec4(aNormal,0.0));
}