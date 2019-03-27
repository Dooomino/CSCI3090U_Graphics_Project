#include <iostream>

struct Planet
{
  float size;
  float distance;
  float speed;
  float period;
};

using namespace std;

Planet planets[] = 
{
//size distance speed period
// MERCURY
  {0.02f,0.76f,1.71f,0.0f},
// VENUS
  {0.06f,1.53f,1.26f,0.0f},
// EARTH
  {0.06f,2.27f,1.07f,0.42f},
// MARS
  {0.03f,2.46f,0.87f,0.41f},
// JUPITER
  {0.69f,2.89f,0.47f,1.01f},
// SATURN
  {0.58f,3.5f,0.35f,0.93f},
// URANUS
  {0.24f,3.8f,0.24f,-0.58f},
// NEPTUNE
  {0.24f,4.3f,0.19f,0.62f},
// PLUTO
  {0.02f,4.8f,0.17f,-0.07f},
};


const GLfloat BlockVertcies[] = {
      -1.0f,-1.0f,-1.0f, 
    -1.0f,-1.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, 
    1.0f, 1.0f,-1.0f, 
    -1.0f,-1.0f,-1.0f,
    -1.0f, 1.0f,-1.0f,
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
    1.0f,-1.0f, 1.0f 
};

// Sun
glm::vec3 sunColor = glm::vec3(1.0,0.12,0.12);

glm::vec3 colorPlatte[]={
// MERCURY
  glm::vec3(0.61,0.85,0.82),
// VENUS
  glm::vec3(1.0,0.71,0.15),
// EARTH
  glm::vec3(0.25,0.25,0.95),
// MARS
  glm::vec3(1.0,0.38,0),
// JUPITER
  glm::vec3(0.91,0.63,0.21),
// SATURN
  glm::vec3(0.95,0.81,0.43),
// URANUS
  glm::vec3(0.17,0.52,0.67),
// NEPTUNE
  glm::vec3(0.65,0.86,0.76),
// PLUTO
  glm::vec3(0.61,0.76,0.65)
};