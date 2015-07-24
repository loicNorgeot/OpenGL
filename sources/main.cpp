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
  mesh_read("/home/loic/Blender/MESH/FauduetBone.o1.mesh", g_vertex, g_normal);

  
//Création des buffers d'attributs
  float color[3] = {0.8f, 0.6f, 0.0f};
  float *g_vertex_buffer_data = new float[g_vertex.size()];
  float *g_color_buffer_data = new float[g_vertex.size()];
  float *g_normal_buffer_data = new float[g_normal.size()];

  cout << "diff de taille = " << g_vertex.size() - g_normal.size() << endl;
  cout << g_vertex.size() << endl;
  cout << g_normal.size() << endl;

  for(int i = 0 ; i < g_vertex.size() ; i+=3){
    for(int j = 0 ; j < 3; j++){
      g_vertex_buffer_data[i+j] = g_vertex[i+j];
      g_color_buffer_data[i+j]  = color[j];
    }
  }
  for(int i = 0 ; i < g_normal.size() ; i++)
    g_normal_buffer_data[i] = g_normal[i];  

  cout << "gnormal = " << g_normal_buffer_data[0] << " " << g_normal_buffer_data[1] << " " << g_normal_buffer_data[2] << endl;
  cout << "gcolor = " << g_color_buffer_data[0] << " " << g_color_buffer_data[1] << " " << g_color_buffer_data[2] << endl;

  
  //Initialisation
  CONTEXT context;
  context.init(width, height, "TESTS OPENGL", 3, 3);

  //Identifiants
  context.programID = LoadShaders("shaders/SimpleVertexShader.vert",
				  "shaders/SimpleFragmentShader.frag");
  context.MatrixID = glGetUniformLocation(context.programID, "MVP"); 
  context.nbVertices = g_vertex.size()/3;

  context.vertexbuffer = context.GL_buffer_data(g_vertex_buffer_data, 3 * context.nbVertices * sizeof(*g_vertex_buffer_data));
  context.colorbuffer  = context.GL_buffer_data(g_color_buffer_data , 3 * context.nbVertices * sizeof(*g_color_buffer_data) );
  context.normalbuffer = context.GL_buffer_data(g_normal_buffer_data, 3 * context.nbVertices * sizeof(*g_normal_buffer_data));

  //Boucle principale
  while( glfwGetKey(context.window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && !glfwWindowShouldClose(context.window)){
    context.loop();
  }

  //Nettoyage
  delete[] g_vertex_buffer_data;
  delete[] g_color_buffer_data;
  delete[] g_normal_buffer_data;
  glfwDestroyWindow(context.window);
  return 1;
}
