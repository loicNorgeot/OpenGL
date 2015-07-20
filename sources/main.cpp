#include <stdio.h> 
#include <stdlib.h>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h> 

#include "shader.h"

using namespace std;

int main(){

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

  cout << "Err " << __LINE__ << ":\t" << glGetError() << endl;
  GLFWwindow* window;

  if(!glfwInit()) { 
    cout << stderr <<  " Failed to initialize GLFW\n" << endl; 
    return 0; 
  }

  cout << "Err " << __LINE__ << ":\t" << glGetError() << endl;
  window = glfwCreateWindow( 1024, 768, "Tutorial 01", NULL, NULL); 
  if( window == NULL ){ 
    cout << stderr << "Failed to open GLFW window" << endl;
    glfwTerminate(); 
    return 0; 
  } 
  glfwMakeContextCurrent(window); 

  cout << "Err " << __LINE__ << ":\t" << glGetError() << endl;
  glewExperimental=GL_TRUE;
  GLenum err = glewInit();
  cout << "Err " << __LINE__ << ":\t" << glGetError() << endl;
  if (err!=GLEW_OK){ 
    cout << glewGetErrorString(err) << endl; 
    return 0; 
  }
  
  cout << "Err " << __LINE__ << ":\t" << glGetError() << endl;
  GLuint VertexArrayID; 
  glGenVertexArrays(1, &VertexArrayID); 
  glBindVertexArray(VertexArrayID);
  cout << "Err " << __LINE__ << ":\t" << glGetError() << endl;

  // S'assure que l'on puisse capturer la touche échap utilisée plus bas
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  cout << "Err " << __LINE__ << ":\t" << glGetError() << endl;



  // Un tableau de trois vecteurs qui représentent trois sommets
  static const GLfloat g_vertex_buffer_data[] = { 
    -1.0f, -1.0f, 0.0f, 
    1.0f, -1.0f, 0.0f, 
    0.0f,  1.0f, 0.0f, 
  };

  GLuint vertexbuffer; 
  // Génère un tampon et place l'identifiant dans 'vertexbuffer'
  glGenBuffers(1, &vertexbuffer); 
  // Les commandes suivantes vont parler de notre tampon 'vertexbuffer'
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer); 
  // Fournit les sommets à OpenGL.
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
 
  GLuint programID = LoadShaders("shaders/SimpleVertexShader.vert",
				 "shaders/SimpleFragmentShader.frag");

  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

  while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && 
	 glfwWindowShouldClose(window) == 0 ){

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Utilise notre shader
    glUseProgram(programID);
    // Dessine le triangle...

    // premier tampon d'attributs : les sommets
    glEnableVertexAttribArray(0); 
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer); 
    glVertexAttribPointer(0,                  // attribut 0. Aucune raison particulière pour 0, mais cela doit correspondre au « layout » dans le shader 
			  3,                  // taille
			  GL_FLOAT,           // type 
			  GL_FALSE,           // normalisé ? 
			  0,                  // nombre d'octets séparant deux sommets dans le tampon
			  (void*)0);          // Décalage du tableau de tampon
 
    // Dessine le triangle ! 
    glDrawArrays(GL_TRIANGLES, 0, 3); // Démarre à partir du sommet 0; 3 sommets au total -> 1 triangle 
 
    glDisableVertexAttribArray(0);
    glUseProgram(0);
    glfwSwapBuffers(window);
  }

  cout << "Err " << __LINE__ << ":\t" << glGetError() << endl;
  return 1;
}
