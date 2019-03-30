# CSCI-3090U Graphics Project
## Author：
- Chen Pi(chen.pi@uoit.net)
- Yudi Tao(yudi.tao@uoit.net)

## Time :
03/29/2019

## Language :
C++ openGL
> **Open Graphics Library (OpenGL)** is a cross-language, cross-platform application programming interface (API) for rendering 2D and 3D vector graphics. The API is typically used to interact with a graphics processing unit (GPU), to achieve hardware-accelerated rendering.

## About project
Our project is about a Flatend solar system. it will show how the planet revolves around the sun.

## How to run the project
```sh
$ git clone https://github.com/Dooomino/CSCI3090U_Graphics_Project
$ cd <project folder>
$ nmake /f Nmakefile.Windows
$ main
```
## Controls:

- Mouse: **Scroll Up/Down** Zoom in/out.
- Mouse: **Left button Drag** rotate by **Axis**.
- Mouse+Key: [**SHIFT**]+**Left button Drag** rotate in **Quaternion** mode.
- Mouse: **Right button Drag** Zoom in/out.

- Key: [ **1 - 9** ] set the foucus planet from **Sun** to **Pluto**.
- Key: [ **F**] toggle ***Focus mode***. 
- Key: [ **=** ] reset camera position.
- Key: [ ** ⟵ & ⟶** ] adjust rotaion speed.
- Key: [ **P** ] pause the animation.
- Key: [ **R** ] reset the inital animation speed.
- Key: [ **C** ] ***DISCO MODE***( Using Cubic Bezier:) )

## Function
#### CreateGeometry Function
This function create the basic geometry of the solar system. 


```
void CreateGeometry(){
  numVertices = mesh.getNumIndexedVertices();

  vertices = mesh.getIndexedPositions();
  uvs = mesh.getIndexedTextureCoords();
  normals = mesh.getIndexedNormals();
  indices = mesh.getTriangleIndices();

....

  glGenBuffers(1,&EBO);
  glBindBuffer(GL_ARRAY_BUFFER,EBO);
  glBufferData(GL_ARRAY_BUFFER,numVertices*sizeof(unsigned int)*3,&indices[0],GL_STATIC_DRAW);
}
```

#### CreateTexture Function
This function create the texture of the planet
```
void CreateTexture(){
  glGenTextures(1,&texture);
  glBindTexture(GL_TEXTURE_2D,texture);

.....

  stbi_image_free(img);
}
```

#### drawPlanet Fuction
This function draw the planet of the solar system.
```
glm::mat4 drawPlanet(glm::mat4 model,float rotate_angle,float self_rotate_speed,float distance,float size,int tilt,float tilt_angle){

.....

}
````

#### loadShaders Function
This function can load the shader for every planet.
```
GLuint loadShaders(){
  GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
  GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
  
  .....
  
    glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
} 
```

#### scroll Function
This function can let solar system scaling on mouse scroll.
```
void scroll(GLFWwindow* window,double xoff,double yoff){
    float distance = yoff* yoff;
    float scaleChange = distance * R_Factor*5;
    if (yoff > 0.0f) {
        scaleChange *= -1.0f;
    }
    scaleFactor += scaleChange;
}
```

#### getTrackballVector Function
This function can get the trackball vector.
```
glm::vec3 getTrackballVector(int x, int y, int width, int height) {
   glm::vec3 P = glm::vec3(1.0 * x / width * 2 - 1.0,
   	                     1.0 * y / height * 2 - 1.0,
   	                     0);
   P.y = -P.y;
   float OP_squared = P.x * P.x + P.y * P.y;
   if (OP_squared <= 1 * 1) {
      P.z = sqrt(1 * 1 - OP_squared);  // Pythagoras
   } else {
      P = glm::normalize(P);  // nearest point
   }
   return P;
}
```

#### drag Function
This function can make the solar system draged by mouse.
```
void drag(GLFWwindow* window, double xpos, double ypos) {

...

}
```

#### keyboard Function
This function perform different function on solar system.
```
void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods){

...

}
```

#### Setfocus Function
This function can set the focus on specific planet.
```
void Setfocus(int planetNum){
  if(isFocus==1){
    float dis = planets[planetNum-1].distance*7;
    float calc_rot_angle = rotate_angle*planets[planetNum-1].speed*animate_speed+sunRotate*rotate_angle;
    CamPosX = dis*glm::cos(-calc_rot_angle-xRotate)*scaleMulti;
    CamPosZ = dis*glm::sin(-calc_rot_angle)*scaleMulti;
  }else{
    CamPosX=0;
    CamPosY=0;
    CamPosZ=0;
  }
}
````

#### resize Function
This function resize the solar system while resize the window.
```
void resize(GLFWwindow* window, int w,int h){
  glViewport(0, 0, w, h);
}
```

#### resetCam Function
This function resets the camera.
```
void resetCam(){

...

}
```

## The reference codes:
- [ObjMesh.cpp](https://github.com/randyfortier/CSCI3090U_Examples/blob/master/04a_ParametricPrimitives_Sphere/ObjMesh.cpp)
- [ObjMesh.h](https://github.com/randyfortier/CSCI3090U_Examples/blob/master/04a_ParametricPrimitives_Sphere/ObjMesh.h)
- [loadShaders()](https://github.com/randyfortier/CSCI3090U_Examples/blob/master/04a_ParametricPrimitives_Sphere/ShaderProgram.cpp)
- [TrackBall.hpp](https://github.com/randyfortier/CSCI3090U_Examples/blob/master/04a_ParametricPrimitives_Sphere/ObjMesh.cpp):
	- getTrackballVector()
	- drag()
 	- mouse()
