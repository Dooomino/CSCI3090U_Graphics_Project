#version 330 core

out vec4 color;

in vec3 fragmentColor;

void main(){
  // color = vec4(1.0, 0.5, 0.2, 1.0);
  color = vec4(0.4,1*fragmentColor.y,0*fragmentColor.y,1.0);
  
}