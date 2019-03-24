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

using namespace std;

GLuint vertexArrayId;
GLuint vertexBuffer;

GLuint colorArrayId;
GLuint colorBuffer;

GLfloat vertcies[] = {
      -1.0f,-1.0f,-1.0f, // triangle 1 : begin
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, // triangle 1 : end
    1.0f, 1.0f,-1.0f, // triangle 2 : begin
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f, // triangle 2 : end
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f,-1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f,-1.0f,
    1.0f,-1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f,-1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f // triangle 1 : end
};

static const GLfloat colorData[] = {
    0.583f,  0.771f,  0.014f,
    0.609f,  0.115f,  0.436f,
    0.327f,  0.483f,  0.844f,
    0.822f,  0.569f,  0.201f,
    0.435f,  0.602f,  0.223f,
    0.310f,  0.747f,  0.185f,
    0.597f,  0.770f,  0.761f,
    0.559f,  0.436f,  0.730f,
    0.359f,  0.583f,  0.152f,
    0.483f,  0.596f,  0.789f,
    0.559f,  0.861f,  0.639f,
    0.195f,  0.548f,  0.859f,
    0.014f,  0.184f,  0.576f,
    0.771f,  0.328f,  0.970f,
    0.406f,  0.615f,  0.116f,
    0.676f,  0.977f,  0.133f,
    0.971f,  0.572f,  0.833f,
    0.140f,  0.616f,  0.489f,
    0.997f,  0.513f,  0.064f,
    0.945f,  0.719f,  0.592f,
    0.543f,  0.021f,  0.978f,
    0.279f,  0.317f,  0.505f,
    0.167f,  0.620f,  0.077f,
    0.347f,  0.857f,  0.137f,
    0.055f,  0.953f,  0.042f,
    0.714f,  0.505f,  0.345f,
    0.783f,  0.290f,  0.734f,
    0.722f,  0.645f,  0.174f,
    0.302f,  0.455f,  0.848f,
    0.225f,  0.587f,  0.040f,
    0.517f,  0.713f,  0.338f,
    0.053f,  0.959f,  0.120f,
    0.393f,  0.621f,  0.362f,
    0.673f,  0.211f,  0.457f,
    0.820f,  0.883f,  0.371f,
    0.982f,  0.099f,  0.879f
};

GLuint loadShaders();
void load(const std::string filename, const bool autoCentre = false, const bool autoNormalize = false);

void mouse(GLFWwindow* window, int button, int action, int mods);
void drag(GLFWwindow* window, double xpos, double ypos);

glm::mat4 drawPlanet(glm::mat4 moldel,float rotate_angle,float self_rotate_speed,float distance,float size,int tilt);

float rotate_angle=0.0f;
float R_Factor=0.1f;
float Fov = 20.0f;

float scaleFactor = 10.0f;
glm::quat rotation = glm::angleAxis(1.0f, glm::vec3(0.0f, 0.0f, 0.0f));
float cameraDistance = 100.0f;

double lxpos=800;
double lypos=600;
glm::vec3 eyepos(0.0f,10.0f,20.0f);

glm::mat4 Projection;
glm::mat4 View;
GLuint MatrixID;

int main(){
  if(!glfwInit()){
    fprintf( stderr, "Failed to initialize GLFW\n" );
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

  GLFWwindow* window;
  window = glfwCreateWindow(800,600,"Hello",NULL,NULL);

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
  //Vetecies Buffer
  glGenVertexArrays(1,&vertexArrayId);
  glBindVertexArray(vertexArrayId);

  glGenBuffers(1,&vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER,vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER,sizeof(vertcies),vertcies,GL_STATIC_DRAW); 
  
  //color Buffer
  glGenVertexArrays(1,&colorArrayId);
  glBindVertexArray(colorArrayId);

  glGenBuffers(1,&colorBuffer);
  glBindBuffer(GL_ARRAY_BUFFER,colorBuffer);
  glBufferData(GL_ARRAY_BUFFER,sizeof(colorData),colorData,GL_STATIC_DRAW);

  //Shaders
  GLuint programid = loadShaders();
  glUseProgram(programid);

  glClearColor(0.0f,0.0f,0.0f,0.0f);


  glEnable(GL_DEPTH_TEST);  
  glDepthFunc(GL_LESS);
  glEnable(GL_CULL_FACE);


  glfwWaitEventsTimeout(1.0f);


  glfwSetMouseButtonCallback(window,mouse);
  glfwSetCursorPosCallback(window,drag);



  //Main loop
  while(!glfwWindowShouldClose(window)){
    //GLM
    Projection = glm::perspective(glm::radians(45.0f), (float)800/ (float)600, 0.1f, 100.0f);

    View = glm::lookAt(
      // glm::vec3(0,1*scaleFactor,1*scaleFactor), 
      Fov+eyepos+scaleFactor, 
      glm::vec3(0,0,0),
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

    mvp = Projection*View*model;
    MatrixID = glGetUniformLocation(programid, "MVP");
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

    glBindBuffer(GL_ARRAY_BUFFER,vertexBuffer);
    glDrawArrays(GL_TRIANGLES,0,12*3);


    glm::mat4 model2 =drawPlanet(model,rotate_angle,2.0f,5.0f,0.2f,0);
    glm::mat4 model3 = drawPlanet(model,2*rotate_angle,1.0f,7.0f,0.3f,0);

    glm::mat4 moon= drawPlanet(model3,rotate_angle,1.0f,7.0f,0.3f,0);

    glm::mat4 model4 = drawPlanet(model,0.5*rotate_angle,0.0f,9.0f,0.2f,0);
    glm::mat4 model5 = drawPlanet(model,0.3*rotate_angle,4.0f,12.0f,0.6f,0);
    glm::mat4 model6 = drawPlanet(model,2*rotate_angle,1.0f,16.0f,0.1f,1);


    glDisableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,(void*)0);     
    

    rotate_angle+=0.01f;
    if(rotate_angle>360){
      rotate_angle=0;
    }


    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

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

glm::mat4 drawPlanet(glm::mat4 model,float rotate_angle,float self_rotate_speed,float distance,float size,int tilt){
    glm::mat4 new_model;
   
    if(tilt==1){
      new_model = glm::rotate(model,rotate_angle,glm::vec3(0.0f,1.0f,0.5f));
    }else{
      new_model = glm::rotate(model,rotate_angle,glm::vec3(0.0f,1.0f,0.0f));
    }

    new_model = glm::translate(new_model,glm::vec3(distance,0.0f,0.0f));
    new_model = glm::rotate(new_model,self_rotate_speed*rotate_angle,glm::vec3(0.0f,1.0f,0.0f));
    

    new_model = glm::scale(new_model,glm::vec3(size));



    glm::mat4 mvp = Projection*View*new_model;
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
    // glDrawArrays(GL_LINES,0,12*3);
    glDrawArrays(GL_TRIANGLES,0,12*3);
    return new_model;
}


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
      } else {
         lxpos = (float)x;
         lypos = (float)y;
      }
   }
}

void mouse(GLFWwindow* window, int button, int action, int mods) {
   if (action == GLFW_RELEASE) {
      lxpos = std::numeric_limits<float>::infinity();
      lypos = std::numeric_limits<float>::infinity();
   }
}

