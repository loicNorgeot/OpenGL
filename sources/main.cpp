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
  std::vector<float> g_vertex, g_normal;
  std::vector<int>   g_indices;
  mesh_read("/home/loic/Blender/MESH/FauduetBone.o1.mesh", g_vertex, g_normal, g_indices);

  //Tableaux a transférer
  //Couleurs
  float colors[3] = {0.8f, 0.6f, 0.0f};
  float *colors_data = new float[g_vertex.size()];
  for(int i = 0 ; i < g_vertex.size() / 3 ; i+=3)
    for(int j = 0 ; j < 3 ; j++)
      colors_data[i+j] = colors[j];
  //Position et normales
  float *vertex_data = &g_vertex[0];
  float *normal_data = &g_normal[0];
  //Indices
  int   *indice_data = &g_indices[0];
  
  //Initialisation
  CONTEXT context;
  context.init(width, height, "TESTS OPENGL", 3, 3);

  //Identifiants
  context.programID     = LoadShaders("shaders/SimpleVertexShader.vert",
				      "shaders/SimpleFragmentShader.frag");
  context.MatrixID      = glGetUniformLocation(context.programID, "MVP"); 
  context.nbVertices    = g_vertex.size()/3;
  context.nbIndices     = g_indices.size();

  //Buffers de données
  context.vertexbuffer  = context.GL_array_buffer( vertex_data, 3);
  context.colorbuffer   = context.GL_array_buffer( colors_data, 3);
  context.normalbuffer  = context.GL_array_buffer( normal_data, 3);
  context.indicesbuffer = context.GL_index_buffer( indice_data, context.nbIndices);
  
  //Boucle principale
  while( glfwGetKey(context.window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && !glfwWindowShouldClose(context.window)){
    context.loop();
  }

  //Nettoyage
  glfwDestroyWindow(context.window);
  delete[] colors_data;
  delete[] vertex_data;
  delete[] normal_data;
  delete[] indice_data;
  return 1;
}














