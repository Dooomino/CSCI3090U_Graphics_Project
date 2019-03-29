#include "Model.h"

Model::Model(){}
Model::Model(const char * path){
  file = fopen(path,"r");
  if( file == NULL ){
    printf("Impossible to open the file !\n");
  }
}

void Model::loadModel( 
vector<glm::vec3>  & out_vertices,
vector<glm::vec2>  & out_uvs,
vector<glm::vec3>  & out_normals)
{
  std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
  std::vector< glm::vec3 > vertices;
  std::vector< glm::vec2 > uvs;
  std::vector< glm::vec3 > normals;
  while(true){
    char line[256];
    int res = fscanf(file, "%s", line);
    if (res == EOF)
        break;

    if(strcmp(line,"v")==0){
      glm::vec3 vertex;
      fscanf(file,"%f %f %f\n",&vertex.x,&vertex.y,&vertex.z);
      vertices.push_back(vertex);
    }else if(strcmp(line,"vt")==0){
      glm::vec2 uv;
      fscanf(file,"%f %f\n",&uv.x,&uv.y);      
      uvs.push_back(uv);
    }else if(strcmp(line,"vn")==0){
      glm::vec3 normal;
      fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
      normals.push_back(normal);
    }else if(strcmp(line,"f")==0){
      std::string vertex1, vertex2, vertex3;
      unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
     
      fscanf(file,"%d/%d/%d %d/%d/%d %d/%d/%d\n",
      &vertexIndex[0],&uvIndex[0],&normalIndex[0],
      &vertexIndex[1],&uvIndex[1],&normalIndex[1],
      &vertexIndex[2],&uvIndex[2],&normalIndex[2]   
      );

      vertexIndices.push_back(vertexIndex[0]);
      vertexIndices.push_back(vertexIndex[1]);
      vertexIndices.push_back(vertexIndex[2]);
      
      uvIndices.push_back(uvIndex[0]);
      uvIndices.push_back(uvIndex[1]);
      uvIndices.push_back(uvIndex[2]);
      
      normalIndices.push_back(normalIndex[0]);
      normalIndices.push_back(normalIndex[1]);
      normalIndices.push_back(normalIndex[2]);

      this->numberVertices++;
    }  
  }

  // printf("Out count %d size %d\n",count,vertexIndices.size());
  this->numberVIndexices = this->numberVertices*3;

  for(unsigned int i=0;i<vertexIndices.size();i++){
    unsigned int vertexIndex=vertexIndices[i];
    glm::vec3 vertex = vertices[vertexIndex-1];
    this->vertices.push_back(vertex);

    unsigned int uvIndex=uvIndices[i];
    glm::vec2 uv = uvs[uvIndex-1];
    this->uvs.push_back(uv);

    unsigned int normalIndex=normalIndices[i];
    glm::vec3 normal = normals[vertexIndex-1];
    this->normals.push_back(normal);
  }

  for (unsigned int i = 0; i < this->numberVIndexices; i++) {
    this->vertexIndeices.push_back(i);
  }

  out_vertices=this->vertices;
  out_uvs=this->uvs;
  out_normals=this->normals;

}