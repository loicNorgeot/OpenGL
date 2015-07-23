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

#include "controls.h"

using namespace std;

void mouse_button_callback( GLFWwindow* window,
			    int button,
			    int action,
			    int mods){
  if ( button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    cout << "toto" << endl;
}
void scroll_callback( GLFWwindow* window,
		      double x,
		      double y){
  if( !FLYINGMODE){
    FOV += y * (-2);
    if( FOV <= 1.0f)
      FOV = 1.0f;
    if( FOV >= 80.0f)
      FOV = 80.0f;
  }
}
void key_callback( GLFWwindow* window,
		   int key,
		   int scancode,
		   int action,
		   int mods){
  
  //Touche P pour nuage de points
  if ( scancode == 33 && action == GLFW_PRESS){
    if(CGL_RENDER==GL_TRIANGLES){
      glPolygonMode(     GL_FRONT, GL_POINT);
      CGL_RENDER = GL_POINTS;
    }
    else
      CGL_RENDER = GL_TRIANGLES;
    cout << "Render: " << CGL_RENDER << endl;
  }

  //Touche Z pour mode wireframe
  if ( scancode == 25 && action == GLFW_PRESS){
    wireframe++;
    if( wireframe == 3)
      wireframe = 0;
    cout << "Wireframe mode: " << wireframe << endl;
  }

  //Touche F pour FLY mode
  if ( key == GLFW_KEY_F && action == GLFW_PRESS){
    //Si on sort du mode normal
    if( !FLYINGMODE)
      ENTERFLYMODE=true;
    FLYINGMODE = !FLYINGMODE;
    cout << "Fly Mode: " << FLYINGMODE << endl;
  }
}

void GUI( GLFWwindow* window){
  //Calcul du deltaTime
  static double lastTime    = glfwGetTime();
  double        currentTime = glfwGetTime(); 
  deltaTime                 = float( currentTime - lastTime);

  //Actions de la GUI
  set_view(        window);
  set_render_type( window);
    
  //Actualisation du temps
  lastTime = currentTime;
}

void set_view( GLFWwindow* window){
  
  glm::vec3 direction;

  //Mode normal
  if( !FLYINGMODE){
    glfwSetInputMode( window, GLFW_CURSOR,      GLFW_CURSOR_NORMAL);
    glfwSetInputMode( window, GLFW_STICKY_KEYS, GL_TRUE);
    direction = -cam * 2.0f;
  }

  //Flying mode
  if(FLYINGMODE){
    //Paramètres globaux
    glfwSetInputMode( window, GLFW_CURSOR,      GLFW_CURSOR_DISABLED);
    glfwSetInputMode( window, GLFW_STICKY_KEYS, GL_FALSE);
    
    //On récupère les données du curseur
    double xpos=0, ypos=0;
    glfwGetCursorPos( window, &xpos, &ypos);

    //A l'entrée
    if( ENTERFLYMODE){
      centerX      = xpos;
      centerY      = ypos;
      ENTERFLYMODE = false;
      //hAngle       = cam;
      //vAngle       = cam;
    }
    
    //Ajustements du FlyingMode
    glfwSetCursorPos(window, centerX, centerY);
    hAngle += mouseSpeed * deltaTime * float( centerX - xpos);
    vAngle += mouseSpeed * deltaTime * float( centerY  - ypos);

    direction =       glm::vec3(  cos(vAngle) * sin( hAngle), 
				  sin(vAngle), 
				  cos(vAngle) * cos( hAngle));
    glm::vec3 right = glm::vec3(  sin(hAngle - 3.14f/2.0f), 
				  0, 
				  cos(hAngle - 3.14f/2.0f));
    glm::vec3 up =    glm::cross( right,
				  direction);

    //Mouvements de la caméra
    float D = deltaTime * speed;
    if ( glfwGetKey( window,  GLFW_KEY_UP )   == GLFW_PRESS)
      cam += direction * D;  
    if ( glfwGetKey( window,  GLFW_KEY_DOWN)  == GLFW_PRESS)
      cam -= direction * D; 
    if ( glfwGetKey( window,  GLFW_KEY_RIGHT) == GLFW_PRESS)
      cam += right     * D; 
    if ( glfwGetKey( window,  GLFW_KEY_LEFT)  == GLFW_PRESS)
      cam -= right     * D; 
  }

  //Actualisation de la visée
  look = cam + direction;  
}

void set_render_type(GLFWwindow* window){
  if( wireframe==0){
    glEnable(      GL_DEPTH_TEST);
    glPolygonMode( GL_FRONT, GL_FILL);
    glEnable(      GL_CULL_FACE);
  }
  if( wireframe==1){
    glPolygonMode( GL_FRONT, GL_LINE);
    glEnable(      GL_CULL_FACE);
    glEnable(      GL_DEPTH_TEST);
  }
  if( wireframe==2){
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
    glDisable(     GL_DEPTH_TEST);
    glDisable(     GL_CULL_FACE);
  }
}


glm::mat4 CONTROLS::update_MVP(){
  glm::mat4 Projection = glm::perspective( FOV,
					   4.0f / 3.0f,
					   0.1f,
					   100.0f); 
  glm::mat4 View       = glm::lookAt(      cam,
					   look,
					   up);
  glm::mat4 Model      = glm::mat4(        1.0f);
  glm::mat4 MVP        = Projection * View * Model;
  return MVP;
}

void CONTROLS::listen( GLFWwindow* window){
  GUI(                        window);
  glfwSetScrollCallback(      window, scroll_callback);
  glfwSetMouseButtonCallback( window, mouse_button_callback);
  glfwSetKeyCallback(         window, key_callback);
}
