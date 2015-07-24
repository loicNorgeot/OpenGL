#include <stdio.h> 
#include <stdlib.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"
#include "controls.h"
#include "context.h"
#include "mesh.h"

using namespace std;
using namespace glm;

int main(){

  //Lecture
  std::vector<float> g_vertex;
  mesh_read("/home/loic/Blender/MESH/FauduetBone.o1.mesh", g_vertex);

  //Création des buffers à envoyer
  float color[3] = {0.0f, 0.0f, 0.8f};
  float *g_vertex_buffer_data = new float[g_vertex.size()];
  float *g_color_buffer_data = new float[g_vertex.size()];
  for(int i = 0 ; i < g_vertex.size() ; i+=3){
    for(int j = 0 ; j < 3; j++){
      g_vertex_buffer_data[i+j] = g_vertex[i+j];
      g_color_buffer_data[i+j]  = color[j];
    }
  }

  //Initialisation
  CONTEXT context;
  context.init(1024, 768, "TESTS OPENGL", 3, 3);

  //Identifiants
  context.programID = LoadShaders("shaders/SimpleVertexShader.vert",
				  "shaders/SimpleFragmentShader.frag");
  context.MatrixID = glGetUniformLocation(context.programID, "MVP"); 
  context.nbVertices = g_vertex.size()/3;

  context.vertexbuffer = context.GL_buffer_data(g_vertex_buffer_data, 3 * context.nbVertices * sizeof(*g_vertex_buffer_data));
  context.colorbuffer  = context.GL_buffer_data(g_color_buffer_data , 3 * context.nbVertices * sizeof(*g_color_buffer_data));

  //Boucle principale
  while( glfwGetKey(context.window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && !glfwWindowShouldClose(context.window)){
    context.loop();
  }
  glfwDestroyWindow(context.window);
  return 1;
}
