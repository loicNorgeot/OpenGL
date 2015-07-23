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
  //Test
  std::vector<float> g_vertex;
  mesh_read("/home/loic/Blender/MESH/FauduetSkin.o1.mesh", g_vertex);
  float g_vertex_buffer_data[g_vertex.size()];
  float g_color_buffer_data[g_vertex.size()];
  for(int i = 0 ; i < g_vertex.size() ; i++){
    g_vertex_buffer_data[i] = g_vertex[i];
    g_color_buffer_data[i] = 1.0f;
  }

  //DEBUG
  cout << g_vertex_buffer_data[10000] << endl;
  cout << g_color_buffer_data[10000] << endl;

  //Initialisation
  CONTEXT context;
  context.init(1024, 768, "TESTS OPENGL", 3, 3);

  //Lecture des shaders
  context.programID = LoadShaders("shaders/SimpleVertexShader.vert",
				  "shaders/SimpleFragmentShader.frag");

  //Identifiant pour la matrice MVP
  context.MatrixID = glGetUniformLocation(context.programID, "MVP"); 

  //context.MVP = context.controls.update_MVP(45.0);

  //Les données sont stockées dans properties.h
  context.nbVertices = sizeof(g_vertex_buffer_data)/(3 * sizeof(*g_vertex_buffer_data));

  //Création des buffers de données
  context.vertexbuffer = context.GL_buffer_data(g_vertex_buffer_data, sizeof(g_vertex_buffer_data));
  context.colorbuffer  = context.GL_buffer_data(g_color_buffer_data , sizeof(g_color_buffer_data ));

  //Boucle principale
  while( glfwGetKey(context.window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && !glfwWindowShouldClose(context.window)){
    context.loop();
  }
  glfwDestroyWindow(context.window);
  return 1;
}
