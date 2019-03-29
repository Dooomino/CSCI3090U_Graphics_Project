#version 330 core
uniform vec3 lightPos;
uniform vec3 objColor;
uniform vec3 lightColor;
uniform int isSun;

out vec4 color;
in vec3 normal;
in vec3 FragPos; 

void main(){
  // vec3 lightColor = vec3(1.0,1.0,1.0);
  if(isSun == 0){
    // vec3 objColor = vec3(0.0,0.5,1.0); 

    float ambientS = 0.1;
    vec3 ambient = ambientS * lightColor;
    
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir),0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient+diffuse) * objColor;
    // vec3 result = objColor;
    // vec3 result = normal;

    color = vec4(result,1.0);
  }else{
    float ambientS = 0.8;
    vec3 ambient = ambientS * lightColor;

    vec3 result = ambient * objColor;

    color = vec4(result,1.0);
  }
}