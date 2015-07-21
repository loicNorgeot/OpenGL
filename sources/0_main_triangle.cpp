#include <stdio.h> 
#include <stdlib.h>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.h"

using namespace std;
using namespace glm;

int main(){
  //Paramètres d'initialisation
  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

  //Initialisation de GLFW
  if(!glfwInit()) { 
    cout << stderr <<  " Failed to initialize GLFW\n" << endl; 
    return 0; 
  }

  //Création de la fenètre via GLFW
  GLFWwindow* window;
  window = glfwCreateWindow( 1024, 768, "Tutorial 01", NULL, NULL); 
  if( window == NULL ){ 
    cout << "Failed to open GLFW window" << endl;
    glfwTerminate(); 
    return 0; 
  } 
  //Linkage au contexte OpenGL
  glfwMakeContextCurrent(window); 

  //Initialisation de GLEW
  glewExperimental=GL_TRUE;
  GLenum err = glewInit();
  if (err!=GLEW_OK){ 
    cout << glewGetErrorString(err) << endl; 
    return 0; 
  }
  
  //Création d'un VAO
  GLuint VertexArrayID; 
  glGenVertexArrays(1, &VertexArrayID); 
  glBindVertexArray(VertexArrayID);

  //Ecoute des inputs claviers
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  
  // Un tableau de trois vecteurs qui représentent trois sommets
  static const GLfloat g_vertex_buffer_data[] = {-1.0f, -1.0f, 0.0f, 
						 1.0f, -1.0f, 0.0f, 
						 0.0f,  1.0f, 0.0f};

  //Lecture des shaders, vert et frag
  GLuint programID = LoadShaders("shaders/SimpleVertexShader.vert",
				 "shaders/SimpleFragmentShader.frag");
  //Identifiant pour la matrice MVP
  GLuint MatrixID = glGetUniformLocation(programID, "MVP"); 

  // Champ de vision de 45°, ration 4:3, distance d'affichage : 0.1 unités <-> 100 unités
  glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f); 
  glm::mat4 View = glm::lookAt(glm::vec3(4,0,3),   // Caméra est à (4,3,3)
			       glm::vec3(0,0,0),   // Regarde l'origine
			       glm::vec3(0,1,0));  // La tête vers le haut
  // Matrice de modèle : ID ici, change pour chaque modèle 
  glm::mat4 Model = glm::mat4(1.0f);
  // ModelViewProjection : multiplication 
  glm::mat4 MVP = Projection * View * Model; // Multiplication en sens inverse

  //Envoi des données à OpenGL
  GLuint vertexbuffer; 
  glGenBuffers(1, &vertexbuffer); 
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer); 
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

  //Spécification d'une couleur de fond
  glClearColor(0.1f, 0.1f, 0.15f, 0.0f);

  //////////////////////////////////////////////////////////////////////////////////////////////////////////
  //BOUCLE PRINCIPALE
  while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && 
	 !glfwWindowShouldClose(window)){//Tant que ni esc ni fermeture
    glfwWaitEvents();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//Nettoyage
    glUseProgram(programID);

    //Transformation au shader actuel dans uniforme "MVP", pour chaque modèle en théorie
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

    //Premier Tampon d'attributs : les sommets
    glEnableVertexAttribArray(0); 
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer); 
    glVertexAttribPointer(0,                  // correspond au « layout » dans le shader 
			  3,                  // taille
			  GL_FLOAT,           // type 
			  GL_FALSE,           // normalisé ? 
			  0,                  // nombre d'octets séparant deux sommets dans le tampon
			  (void*)0);          // Décalage du tableau de tampon
 
    //Dessin
    glDrawArrays(GL_TRIANGLES, 0, 3); //Du sommet 0, pour 3 sommets
 
    //Fin et nettoyage
    glDisableVertexAttribArray(0);
    glUseProgram(0);
    glfwSwapBuffers(window);
  }
  glfwDestroyWindow(window);
  return 1;
}


//cout << "Err " << __LINE__ << ":\t" << glGetError() << endl;
