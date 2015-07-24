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
  int nbVertices;
  glm::mat4 MVP;
  CONTROLS controls;

  int init(int sizeX, int sizeY, std::string windowName, int major, int minor);
  GLuint GL_buffer_data(const float* p_data, int mem_size);
  void GL_attributes(GLuint buffer, int index, std::string name);
  void loop();
};

#endif
