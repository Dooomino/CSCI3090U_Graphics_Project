#include <iostream>
#include <vector>
#include <cstdio>
#include <string>
#include <cstring>

#include <glm/glm.hpp>

using namespace std;

class Model
{
private:
  FILE* file;
public:
  Model(const char * path);
  void loadModel( 
  vector<glm::vec3>  & out_vertices,
  vector<glm::vec2>  & out_uvs,
  vector<glm::vec3>  & out_normals
  );
};

