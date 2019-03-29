#version 330 core

uniform vec3 lightPos;
uniform vec3 objColor;
uniform vec3 lightColor;
uniform int isSun;

in vec3 normal;
in vec3 FragPos; 
in vec3 outPosition;

in vec2 textureCoord;
uniform sampler2D texture;

void main(){
  // vec3 lightColor = vec3(1.0,1.0,1.0);
  if(isSun != 1){
    // vec3 objColor = vec3(0.0,0.5,1.0); 

    float ambientS = 0.1;
    vec3 ambient = ambientS * lightColor;
    
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir),0.0);
    vec3 diffuse = diff * lightColor;

    // vec3 result = objColor;
    // vec3 result = normal;
    vec3 result;
    if(isSun ==2){
      result = (ambient+diffuse) * vec3(texture2D(texture, textureCoord));
    }else{
      result = (ambient+diffuse) * objColor;  
    }
    gl_FragColor = vec4(result,1.0);      
  }else{
    float ambientS = 0.8;
    // vec3 ambient = ambientS * vec3(1.0,lightColor.y,lightColor.z);
    vec3 ambient = ambientS * lightColor;

    float dx = abs(outPosition.x);
    float dz = abs(outPosition.z);
    float dy = abs(outPosition.y);
    
    float dist = dx * dz * dy;

    vec3 result = ambient * objColor;

    gl_FragColor = vec4(result,1.0);
    // gl_FragColor =texture2D(texture, textureCoord);
  }
}
