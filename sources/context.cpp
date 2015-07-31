/**
 * \file      context.cpp
 * \author    Norgeot
 * \brief     Implements the functions corresponding to the OpenGL context.
 *
 * \details   This class wraps the OpenGL context and window methods,
 * allowing the internal mechanics not to be seen for the user.
 */

#include <stdio.h> 
#include <stdlib.h>
#include <iostream>
#include <string>
#include <cstdlib>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "context.h"

using namespace std;

int CONTEXT::init( int sizeX,
		   int sizeY,
		   string windowName,
		   int major,
		   int minor){
  /** 
   * \brief       Initialise les contextes GLFW et GLEW,
   *  et créé fenetre et VAO.
   *
   * \details     Proceeds to GLFWInit(), then creates the window   
   * and puts it in the current context.
   * GLEW is then initialized, before the definition of some basic OpenGL 
   * properties, as background color... etc
   * \param   sizeX       window width
   * \param   sizeY       window height
   * \param   windowName  window title
   * \param   major       OPENGL version major
   * \param   minor       OPENGL version minor
   * \return  Un \e int representant le statut d'éxécution
   */

  //GLFW Initialization
  glfwWindowHint( GLFW_SAMPLES,               4);
  glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, major);
  glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, minor); 
  glfwWindowHint( GLFW_OPENGL_PROFILE,        GLFW_OPENGL_CORE_PROFILE); 
  if( !glfwInit()) { 
    cout << " Failed to initialize GLFW\n" << endl; 
    return 0; 
  }

  //GLFW window creation
  window = glfwCreateWindow( sizeX, sizeY, windowName.c_str(), NULL, NULL); 
  if( window == NULL){ 
    cout << "Failed to open GLFW window" << endl;
    cout << glGetError()                 << endl;
    glfwTerminate(); 
    return 0;
  } 
  glfwSetInputMode( window, GLFW_STICKY_KEYS, GL_TRUE);
  glfwMakeContextCurrent(window);

  //GLEW Initialization
  glewExperimental=GL_TRUE;
  GLenum err = glewInit();
  if ( err!=GLEW_OK){ 
    cout << glewGetErrorString( err) << endl; 
    return 0; 
  }
  //VAO, color and depth
  glGenVertexArrays( 1,        &VertexArrayID); 
  glBindVertexArray( VertexArrayID);
  glClearColor(      0.0f,     0.0f, 0.0f, 0.0f);
  glEnable(          GL_DEPTH_TEST); 
  glEnable(          GL_CULL_FACE);
  glDepthFunc(       GL_LESS);
  glPolygonMode(     GL_FRONT, GL_FILL);
  glEnable(          GL_MULTISAMPLE);  
  
  return 1;
}

//Send data to OpenGL
GLuint CONTEXT::GL_array_buffer( const float* p_data,
				int dimension){
  GLuint buffer;
  int mem = dimension * sizeof(float) * nbVertices;
  glGenBuffers( 1,               &buffer); 
  glBindBuffer( GL_ARRAY_BUFFER, buffer); 
  glBufferData( GL_ARRAY_BUFFER, mem, p_data, GL_STATIC_DRAW);
  return buffer;
}
GLuint CONTEXT::GL_index_buffer( const int* indices){
  GLuint buffer;
  int mem = nbIndices * sizeof(int);
  glGenBuffers( 1,               &buffer); 
  glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, buffer); 
  glBufferData( GL_ELEMENT_ARRAY_BUFFER, mem, indices, GL_STATIC_DRAW);
  return buffer;
}

//Link to shaders
void CONTEXT::GL_attributes( GLuint buffer,
			     int index,
			     int dimension,
			     string name){
  glEnableVertexAttribArray( index); 
  glBindBuffer(              GL_ARRAY_BUFFER, buffer); 
  glVertexAttribPointer(     index, dimension, GL_FLOAT, GL_FALSE, 0, ( void*)0);
  glBindAttribLocation(      programID, index, name.c_str());
}

void CONTEXT::loop(){
  while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && !glfwWindowShouldClose(window)){
    glfwWaitEvents();
    glClear(            GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(       programID);
    
    controls.listen(    window);
    MVP = controls.update_MVP();
    
    //Envoi des uniformes
    glUniformMatrix4fv( MatrixID, 1, GL_FALSE, &MVP[0][0]);
    
    //Envoi de tous les buffers
    int index = glGetUniformLocation(programID, "renderMode");
    int colorVariable = glGetUniformLocation(programID, "useColor");
    glUniform1f(colorVariable, useColor);

    //Linkage des attributs
    GL_attributes(      vertexbuffer, 0, 3, "vertexPosition_modelspace");
    if(RENDER=="MESH_SOL"){
      GL_attributes(      colorbuffer , 1, 1, "vertexColor");
      GL_attributes(      normalbuffer, 2, 3, "normals");
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesbuffer);
    }
    if(RENDER == "PLY_UV")
      GL_attributes(uvbuffer, 1, 2, "UV");
    if(RENDER == "PLY_COLORS"){
      GL_attributes(colorbuffer, 1, 3, "vertexColor");
    }
    
    //Rendu avec faces colorées
    if(render_mode==0){
      wireframe=false;
      glUniform1f(index, 0.0f);
      if(RENDER!="PLY_UV")
	glDrawElements( CGL_RENDER, nbIndices, GL_UNSIGNED_INT, (void*)0);
      else
	glDrawArrays(CGL_RENDER, 0, nbVertices);
      
      glDepthFunc(   GL_LESS);
      glFrontFace(GL_CW);
      glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
      glLineWidth(2.0f);
      glUniform1f(index, 1.0f);
      if(RENDER!="PLY_UV")
	glDrawElements( CGL_RENDER, nbIndices, GL_UNSIGNED_INT, (void*)0);
      else
	glDrawArrays(CGL_RENDER, 0, nbVertices);
    }
    //Rendu en mode wireframe
    else if (render_mode==1 || render_mode==2){
      wireframe=true;
      glLineWidth(1.0f);
      glUniform1f(index, 2.0f);
      if(RENDER!="PLY_UV")
	glDrawElements( CGL_RENDER, nbIndices, GL_UNSIGNED_INT, (void*)0);
      else
	glDrawArrays(CGL_RENDER, 0, nbVertices);
    }
    //Rendu en nuage de points
    else if (render_mode == 3){
      if(RENDER!="PLY_UV")
	glDrawElements( CGL_RENDER, nbIndices, GL_UNSIGNED_INT, (void*)0);
      else
	glDrawArrays(CGL_RENDER, 0, nbVertices);
    }
    
    //Fin et nettoyage
    glDisableVertexAttribArray( 0);
    glDisableVertexAttribArray( 1);
    glDisableVertexAttribArray( 2);
    glUseProgram(               0);
    glfwSwapBuffers(            window);
  }
}
