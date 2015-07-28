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
  std::vector<float> g_vertex, g_normal, g_solution;
  std::vector<int>   g_indices;
  mesh_read("/home/loic/Blender/MESH/FauduetBone.o1.mesh", 
	    "/home/loic/toto.sol",
	    g_vertex, 
	    g_normal, 
	    g_indices,
	    g_solution);
  cout << "yeah = " << g_solution.size() << endl;
  cout << g_vertex.size() << endl;

  //Tableaux a transférer
  //Couleurs
  //float colors[3] = {0.8f, 0.6f, 0.0f};
  float *scalar_data = &g_solution[0];
  //for(int i = 0 ; i < g_solution.size() ; i+=3){
  //colors_data[i + 0] = g_solution[i];
  //colors_data[i + 1] = 0.0f;
  //colors_data[i + 2] = 1.0f - g_solution[i];
  //}
  //colors_data[3*i + j] = colors[j];
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
  context.colorbuffer   = context.GL_array_buffer( scalar_data, 1);
  context.normalbuffer  = context.GL_array_buffer( normal_data, 3);
  context.indicesbuffer = context.GL_index_buffer( indice_data, context.nbIndices);
  
  //Boucle principale
  while( glfwGetKey(context.window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && !glfwWindowShouldClose(context.window)){
    context.loop();
  }

  //Nettoyage
  glfwDestroyWindow(context.window);
  delete[] scalar_data;
  delete[] vertex_data;
  delete[] normal_data;
  delete[] indice_data;
  return 1;
}














