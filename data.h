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
  {0.02f,1.76f,1.71f,0.0f},
// VENUS
  {0.06f,2.03f,1.26f,0.0f},
// EARTH
  {0.06f,2.17f,1.07f,0.42f},
// MARS
  {0.03f,2.36f,0.87f,0.41f},
// JUPITER
  {0.69f,2.89f,0.47f,1.01f},
// SATURN
  {0.58f,3.16f,0.35f,0.93f},
// URANUS
  {0.24f,3.46f,0.24f,-0.58f},
// NEPTUNE
  {0.24f,3.65f,0.19f,0.62f},
// PLUTO
  {0.02f,3.77f,0.17f,-0.07f},
};