#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>

#include "data.h"
#include "Model.h"

using namespace std;

float width = 1366;
float height = 768; 

float CamPosX =0.0f,CamPosY=0.0f,CamPosZ =0.0f;

GLuint loadShaders();
void load(const std::string filename, const bool autoCentre = false, const bool autoNormalize = false);

void mouse(GLFWwindow* window, int button, int action, int mods);
void drag(GLFWwindow* window, double xpos, double ypos);
void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods);

void scroll(GLFWwindow* window,double xoff,double yoff);

glm::mat4 drawPlanet(glm::mat4 moldel,float rotate_angle,float self_rotate_speed,float distance,float size,int tilt,float tilt_angle);

void Setfocus(int planetNum);
void resetCam();


float rotate_angle=0.0f;
float rotate_accel=0.0f;
float xRotate=0.0f;
float yRotate=0.0f;
float zRotate=0.0f;
float R_Factor=0.1f;
float Fov = 20.0f;
float animate_speed=2.0f;

float scaleFactor = 10.0f;
glm::quat rotation = glm::angleAxis(1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
float cameraDistance = 100.0f;

double lxpos=800,lypos=600;
float lxscf=0.0f, lyscf=0.0f;
float lx=0,lz=0;
glm::vec3 eyepos(0.0f,10.0f,20.0f);

glm::mat4 Projection;
glm::mat4 View;
GLuint MatrixID;
GLuint ModelID;
GLuint LightPosID;
GLuint objcID;
GLuint isSunID;

GLint numVertices;

GLuint vertexArrayId;
GLuint vertexBuffer;
GLuint colorArrayId;
GLuint colorBuffer;

std::vector< glm::vec3 > vertices;
std::vector< glm::vec2 > uvs;
std::vector< glm::vec3 > normals;

int QuartScroll = 0;
int isFocus = 0;
glm::vec3 originEye;
int FocusOn_Num=0;

float scaleMulti = 2.0f;

int main(){
  if(!glfwInit()){
    fprintf( stderr, "Failed to initialize GLFW\n" );
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
  glfwWindowHint(GLFW_SAMPLES, 4);
  
  GLFWwindow* window;
  window = glfwCreateWindow(width,height,"Hello",NULL,NULL);

  if (window == NULL)
  {
    fprintf( stderr, "Failed to open GLFW window\n" );
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glewExperimental=true;

  if (glewInit() != GLEW_OK) {
    fprintf(stderr, "Failed to initialize GLEW\n");
    return -1;
  }
  // END INIT

  // load Model
  // Model modeloder("Meshes/my_sphere.obj");
  Model modeloder("Meshes/cube.obj");
  // Model modeloder("Meshes/trous.obj");

  modeloder.loadModel(vertices, uvs, normals);

  //Vetecies Buffer
  glGenVertexArrays(1,&vertexArrayId);
  glBindVertexArray(vertexArrayId);

  glGenBuffers(1,&vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER,vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER,vertices.size() * sizeof(glm::vec3),&vertices[0],GL_STATIC_DRAW); 
  
  //Normal Buffer
  GLuint normalBuffer;
  glGenBuffers(1,&normalBuffer);
  glBindBuffer(GL_ARRAY_BUFFER,normalBuffer);
  glBufferData(GL_ARRAY_BUFFER,normals.size() * sizeof(glm::vec3),&normals[0],GL_STATIC_DRAW);

  glEnableVertexAttribArray(2);
  glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
  glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,0,(void*)0);


  // //color Buffer
  // glGenVertexArrays(1,&colorArrayId);
  // glBindVertexArray(colorArrayId);

  // glGenBuffers(1,&colorBuffer);
  // glBindBuffer(GL_ARRAY_BUFFER,colorBuffer);
  // glBufferData(GL_ARRAY_BUFFER,sizeof(colorData),colorData,GL_STATIC_DRAW);

  //Shaders
  GLuint programid = loadShaders();
  glUseProgram(programid);

  glClearColor(0.0f,0.0f,0.0f,0.0f);


  glEnable(GL_DEPTH_TEST); 
  glDepthFunc(GL_LESS);
  glEnable(GL_CULL_FACE);
  glEnable(GL_MULTISAMPLE); 


  glfwWaitEventsTimeout(1.0f);


  glfwSetMouseButtonCallback(window,mouse);
  glfwSetCursorPosCallback(window,drag);
  glfwSetKeyCallback(window,keyboard);
  glfwSetScrollCallback(window,scroll);

  originEye =  Fov+eyepos+scaleFactor;

    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //Main loop
  while(!glfwWindowShouldClose(window)){
    //GLM
    Projection = glm::perspective(glm::radians(45.0f), width/ height, 0.1f, 1000.0f);
    
    Setfocus(FocusOn_Num);

    View = glm::lookAt(
      // glm::vec3(0,1*scaleFactor,1*scaleFactor), 
      Fov+eyepos+scaleFactor, 
      glm::vec3(CamPosX,CamPosY,CamPosZ),
      glm::vec3(0,1,0) 
    );


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
      glfwSetWindowShouldClose(window,true);
    }

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);  

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 mvp;

    model = glm::rotate(model,xRotate,glm::vec3(1.0f,0.0f,0.0f));
    model = glm::rotate(model,yRotate,glm::vec3(0.0f,1.0f,0.0f));
    model = glm::scale(model,glm::vec3(scaleMulti));

    // model = glm::rotate(model,zRotate,glm::vec3(0.0f,0.0f,1.0f));
    // model = glm::translate(model,glm::vec3(0.0f,-2.0f,0.0f));


    mvp = Projection*View*model;
    MatrixID = glGetUniformLocation(programid, "MVP");
    ModelID = glGetUniformLocation(programid, "model");
    objcID = glGetUniformLocation(programid, "objColor");
    LightPosID = glGetUniformLocation(programid, "lightPos");
    isSunID = glGetUniformLocation(programid, "isSun");
    

    // glUniform3f(LightPosID,
    // 20+glm::cos(glm::radians(lx))+glm::sin(glm::radians(lz)),
    // 0,
    // 20+-glm::sin(glm::radians(lx))+glm::cos(glm::radians(lz)));
  
    // lx+=0.5f;
    // lz+=0.5f;
    glUniform3f(LightPosID,0,10,-10);
    glUniform3f(objcID,sunColor.x,sunColor.y,sunColor.z);

    glUniform1i(isSunID,1);
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
    // glUniformMatrix4fv(ModelID, 1, GL_FALSE, &model[0][0]);

    glBindBuffer(GL_ARRAY_BUFFER,vertexBuffer);
    
    // glDrawArrays(GL_LINES,0,sizeof(float)*vertices.size()+2);
    glDrawArrays(GL_TRIANGLES,0,sizeof(float)*vertices.size()+2);
	  
    
    glm::mat4 models[9];

    for (int i =0 ;i<9;i++){
      int tilt=0;
      float tiltang=0;
      float dis = planets[i].distance*7;
      float calc_rot_angle = rotate_angle*planets[i].speed*animate_speed;

      glUniform3f(objcID,colorPlatte[i].x,colorPlatte[i].y,colorPlatte[i].z);
      if(i==8){
        tilt=1;
        tiltang=45.0f;
      }
      if(planets[i].size<0.1){
        models[i] =drawPlanet(model,calc_rot_angle,planets[i].period,dis,planets[i].size+0.2,tilt,tiltang);        
      }else
      {
        models[i] =drawPlanet(model,calc_rot_angle,planets[i].period,dis,planets[i].size,tilt,tiltang);
      }
    }

    glm::mat4 modelmoon = drawPlanet(models[2],rotate_angle*0.5f,2.0f,5.0f,0.3,0,0);

    glUniformMatrix4fv(ModelID, 1, GL_FALSE, &model[0][0]);

    glDisableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,(void*)0);     
    

    rotate_angle+=0.01f+rotate_accel;
    if(rotate_angle>360 || rotate_angle<0){
      rotate_angle=0;
    }


    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

glm::mat4 drawPlanet(glm::mat4 model,float rotate_angle,float self_rotate_speed,float distance,float size,int tilt,float tilt_angle){
    glm::mat4 new_model;
    glUniform1i(isSunID,0);
    if(tilt==1){
      new_model = glm::rotate(model,rotate_angle,glm::vec3(0.0f,1.0f-tilt_angle/360.0f,tilt_angle/360.0f));
    }else{
      new_model = glm::rotate(model,rotate_angle,glm::vec3(0.0f,1.0f,0.0f));
    }

    new_model = glm::translate(new_model,glm::vec3(distance,0.0f,0.0f));
    new_model = glm::rotate(new_model,self_rotate_speed*rotate_angle,glm::vec3(0.0f,1.0f,0.0f));
    
    new_model = glm::scale(new_model,glm::vec3(size));

    glm::mat4 mvp = Projection*View*new_model;
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
    glUniformMatrix4fv(ModelID, 1, GL_FALSE, &new_model[0][0]);
    glDrawArrays(GL_TRIANGLES,0,sizeof(float)*vertices.size()+2);
    // glDrawArrays(GL_LINES,0,sizeof(float)*vertices.size()+2);

    return new_model;
}

/**
 * integrated from 
 * https://github.com/randyfortier/CSCI3090U_Examples/tree/master/11b_TextureMapping_SkyBox_QuaternionTrackball
 * Trackball.hpp  ShaderProgram.cpp ShaderProgram.h
*/

GLuint loadShaders(){
  GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
  GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

  std::string vShaderSource;
  std::string vLine;
	std::ifstream Vertexfile("shader/vertex.fx", std::ios::in);
	if(Vertexfile.is_open()){
		 while (getline(Vertexfile, vLine)) {
      vShaderSource.append(vLine);
      vShaderSource.append("\n");
    }
	}else{
		getchar();
		return 0;
	}

  std::string fShaderSource;
  std::string fLine;
	std::ifstream Fragfile("shader/frag.fx", std::ios::in);
	if(Fragfile.is_open()){
		 while (getline(Fragfile, fLine)) {
      fShaderSource.append(fLine);
      fShaderSource.append("\n");
    }
	}else{
		getchar();
		return 0;
	}

  GLint Result = GL_FALSE;
	int InfoLogLength;

  char const * VertexSourcePointer = vShaderSource.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

  glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

  char const * FragmentSourcePointer = fShaderSource.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}


  GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;

} 
void scroll(GLFWwindow* window,double xoff,double yoff){
    float distance = yoff* yoff;
    float scaleChange = distance * R_Factor*5;
    if (yoff > 0.0f) {
        scaleChange *= -1.0f;
    }
    scaleFactor += scaleChange;
}
/**
 * integrated from 
 * https://github.com/randyfortier/CSCI3090U_Examples/tree/master/11b_TextureMapping_SkyBox_QuaternionTrackball
 * Trackball.hpp
 * 
*/
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

/**
 * integrated from 
 * https://github.com/randyfortier/CSCI3090U_Examples/tree/master/11b_TextureMapping_SkyBox_QuaternionTrackball
 * Trackball.hpp
 * 
 * Normal: Axis View Rotaion
 * Shift: Quaternion View Rotation 
 * 
*/
void drag(GLFWwindow* window, double xpos, double ypos) {
   int width, height;
   glfwGetFramebufferSize(window, &width, &height);

   int stateLeft = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
   int stateRight = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);

   bool areScaling = stateRight == GLFW_PRESS;
   bool mouseDown = (stateLeft == GLFW_PRESS) || (stateRight == GLFW_PRESS);

   if (mouseDown) {
      int x = (int)xpos;
      int y = (int)ypos;
      if (!std::isinf(lxpos) && !std::isinf(lypos)) {
         if (areScaling) {
            float dx = lxpos - (float)x;
            float dy = lypos - (float)y;
            // scale in/out (right button drag)
            float distance = sqrt(dx * dx + dy * dy);
            float scaleChange = distance * R_Factor;
            if ((dx + dy) > 0.0f) {
               scaleChange *= -1.0f;
            }
            printf("%f %f\n",scaleFactor,scaleChange);
            
            scaleFactor += scaleChange;
        
            lxpos = x;
            lypos = y;
         } else {
          if(QuartScroll == 0){
            float dx = lxpos - (float)x;
            float dy = lypos - (float)y;

            float xrot = glm::abs(dx/100);
            if(dx>0){
              yRotate +=xrot;
            } else{
              yRotate -=xrot;
            }            

            float yrot = glm::abs(dy/100);             
            if(dy<0){             
              xRotate += yrot;
            }else{
              xRotate -= yrot;
            }
            

            lxpos = (float)x;
            lypos = (float)y;
          }else{
              // trackball rotation (left button drag)
              glm::vec3 a = getTrackballVector(lxpos, lypos, width, height);
              glm::vec3 b = getTrackballVector(x, y, width, height);

              float rotateSpeed = 1.0f;
              float angle = 1.0f * -(float)acos(glm::dot(a, b) / glm::length(a) / glm::length(b));

              if (!std::isnan(angle)) {
                glm::vec3 axis = glm::normalize(glm::cross(a, b));
                if (std::isnan(axis.x) || std::isnan(axis.y) || std::isnan(axis.z)) {
                    return;
                }
                angle *= rotateSpeed;
                glm::quat quaternion = glm::angleAxis(angle, axis);

                //glm::mat4 rotationMatrix = glm::mat4_cast(quaternion);
                eyepos = eyepos * quaternion;
                lxpos = (float)x;
                lypos = (float)y;

              }

            }
         }
      } else {
         lxpos = (float)x;
         lypos = (float)y;
      }
   }
}
/**
 * integrated from 
 * https://github.com/randyfortier/CSCI3090U_Examples/tree/master/11b_TextureMapping_SkyBox_QuaternionTrackball
 * Trackball.hpp
*/

void mouse(GLFWwindow* window, int button, int action, int mods) {
   if (action == GLFW_RELEASE) {
      lxpos = std::numeric_limits<float>::infinity();
      lypos = std::numeric_limits<float>::infinity();
   }
   if(action==GLFW_PRESS){ 
    if(button==GLFW_MOUSE_BUTTON_LEFT && QuartScroll==0){
      resetCam();
    }
   }
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods){
  if(action == GLFW_REPEAT){
    if(key == GLFW_KEY_RIGHT){
      rotate_accel+=0.005f;
    } else if(key == GLFW_KEY_LEFT){
      rotate_accel-=0.005f;
    }
  }else if(action == GLFW_PRESS){
    if(key == GLFW_KEY_RIGHT){
      rotate_accel+=0.005f;
    } else if(key == GLFW_KEY_LEFT){
      rotate_accel-=0.005f;
    }else if(key==GLFW_KEY_R){
      rotate_accel=0.0f;
    }else if(key==GLFW_KEY_F){
      printf("Focus ");
      if(isFocus==0){
        resetCam();
        isFocus=1;
        printf("On\n");
      }else{
        resetCam();
        isFocus=0;
        printf("Off\n");
      }
    }else if(key==GLFW_KEY_KP_EQUAL){
      resetCam();
    }else if(key==GLFW_KEY_LEFT_SHIFT || key==GLFW_KEY_RIGHT_SHIFT){
      printf("Shift\n");
      QuartScroll=1;
    }else{
      switch (key){
        case GLFW_KEY_1:
          FocusOn_Num=1;
          break;
        case GLFW_KEY_2:
          FocusOn_Num=2;
          break;
        case GLFW_KEY_3:
          FocusOn_Num=3;
          break;
        case GLFW_KEY_4:
          FocusOn_Num=4;
          break;
        case GLFW_KEY_5:
          FocusOn_Num=5;
          break;
        case GLFW_KEY_6:
          FocusOn_Num=6;
          break;
        case GLFW_KEY_7:
          FocusOn_Num=7;
          break;
        case GLFW_KEY_8:
          FocusOn_Num=8;
          break;
        case GLFW_KEY_9:
          FocusOn_Num=9;
          break;
        default:
          FocusOn_Num=0;
          break;
      }
    }
  }else{
    QuartScroll=0;
  }
}

void Setfocus(int planetNum){
  if(isFocus==1){
    float dis = planets[planetNum-1].distance*7;
    float calc_rot_angle = rotate_angle*planets[planetNum-1].speed*animate_speed;
    CamPosX = dis*glm::cos(-calc_rot_angle-xRotate)*scaleMulti;
    CamPosZ = dis*glm::sin(-calc_rot_angle)*scaleMulti;
  }else{
    CamPosX=0;
    CamPosY=0;
    CamPosZ=0;
  }
}

void resetCam(){
  CamPosX=0;
  CamPosZ=0;
  CamPosY=0;
  View = glm::lookAt(
    // glm::vec3(0,1*scaleFactor,1*scaleFactor), 
    originEye, 
    glm::vec3(CamPosX,CamPosY,CamPosZ),
    glm::vec3(0,1,0) 
  );
  xRotate=0;
  yRotate=0;
  zRotate=0;
  FocusOn_Num=0;
  isFocus=0;
}
