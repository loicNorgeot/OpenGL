#ifndef CONTEXTHEADERDEF
#define CONTEXTHEADERDEF

#include <string>
#include "controls.h"

class CONTEXT{
public:
  GLFWwindow* window;

  GLuint VertexArrayID;
  GLuint programID;
  GLuint MatrixID;
  GLuint vertexbuffer;
  GLuint colorbuffer;
  GLuint normalbuffer;
  GLuint indicesbuffer;

  int nbVertices;
  int nbIndices;

  glm::mat4 MVP;

  CONTROLS controls;

  int init(int sizeX, int sizeY, std::string windowName, int major, int minor);
  GLuint GL_array_buffer(const float* p_data, int dimension);
  GLuint GL_index_buffer( const int* indices,int dimension);
  void GL_attributes(GLuint buffer, int index, std::string name);
  void loop();
};

#endif
