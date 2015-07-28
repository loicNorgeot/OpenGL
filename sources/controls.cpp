#include <stdio.h> 
#include <stdlib.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include "controls.h"

int    render_mode    = 0;
int    useColor       = true;
bool   wireframe      = false;
GLenum CGL_RENDER     = GL_POINTS;
int    width          = 1024;
int    height         = 768;

using namespace std;

void mouse_button_callback( GLFWwindow* window,
			    int button,
			    int action,
			    int mods){
  //Vitesse de déplacement en mode FLYING
  if ( button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    speed = 0.05f;
  if ( button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    speed = 0.5f;

  //En mode statique, on change cam en fonction du mouvement de la souris, une fois pressée:
  if ( button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    enterRotating=true;
  if ( glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS )
    rotating = true;
  else
    rotating=false;

}
void scroll_callback( GLFWwindow* window,
		      double x,
		      double y){
  //En mode statique, on zoome
  if( !FLYINGMODE){
    float D = 0.05f;
    cam -= cam * D * float(y);  
  }
  //En mode vol, on monte et descend
  if(FLYINGMODE){
    float D = 0.01f;
    cam += glm::vec3(0, D*y, 0);
  }
}
void key_callback( GLFWwindow* window,
		   int key,
		   int scancode,
		   int action,
		   int mods){
  
  //Touche C pour le toogle couleur
  if ( key == GLFW_KEY_C && action == GLFW_PRESS){
      useColor=!useColor;
    cout << "Use color: " << useColor << endl;
  }
  
  //Touche Z pour mode wireframe
  if ( scancode == 25 && action == GLFW_PRESS){
    render_mode++;
    if(render_mode==3)
      render_mode=0;
    cout << "Render mode: " << render_mode << endl;
  }

  //Touche F pour FLY mode
  if ( key == GLFW_KEY_F && action == GLFW_PRESS){
    //Si on sort du mode normal
    if( !FLYINGMODE)
      ENTERFLYMODE=true;
    FLYINGMODE = !FLYINGMODE;
    cout << "Fly Mode: " << FLYINGMODE << endl;
  }

  //Touche S pour screenshot
  if ( key == GLFW_KEY_S && action == GLFW_PRESS)
    screenshot();
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
    //Paramètres 
    glfwSetInputMode( window, GLFW_CURSOR,      GLFW_CURSOR_NORMAL);
    glfwSetInputMode( window, GLFW_STICKY_KEYS, GL_TRUE);
    //On récupère les données du curseur
    double xpos=0, ypos=0;
    glfwGetCursorPos( window, &xpos, &ypos);
    //Modification de l'emplacement de la caméra
    if(rotating){
      if(enterRotating){
	centerX = xpos; 
	centerY = ypos;
	enterRotating=false;
      }
      
      float sensibility = 15.0f;
      //Rotation autour de l'axe vertical
      glm::quat quaternionY = glm::angleAxis(float(sensibility * deltaTime * (centerX - xpos)), 
					     glm::vec3(0.0f, 1.0f, 0.0f));
      //Rotation autour de l'axe "droite" par rapport à la caméra
      glm::quat quaternionX = glm::angleAxis(float(sensibility * deltaTime * (centerY - ypos)), 
					     glm::cross(-cam, glm::vec3(0.0f, 1.0f, 0.0f)));
      centerX = xpos; 
      centerY = ypos;

      glm::mat4 rotationY   = glm::toMat4(quaternionY);
      glm::mat4 rotationX   = glm::toMat4(quaternionX);
      cam = glm::vec3( rotationY * rotationX * glm::vec4(cam,1));
    }

    //Actualisation de la direction de la caméra
    direction = -cam * 2.0f;
    look = cam + direction;  
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
      if(FIRSTENTER){
	FIRSTENTER=false;
      }
    }
    
    //Ajustements du FlyingMode
    glm::quat horizontalQuat = glm::angleAxis(mouseSpeed * 1.0f * float(centerX - xpos),
					   glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 horizontalRot   = glm::toMat4(horizontalQuat);
    glm::vec3 right = glm::cross(look, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat verticalQuat = glm::angleAxis(mouseSpeed * 1.0f * float(centerY - ypos), right);
    glm::mat4 verticalRot   = glm::toMat4(verticalQuat);

    //Actualisation de la visée
    look = glm::vec3( verticalRot * horizontalRot * glm::vec4(look,1.0f) );

    //On remet le curseur au milieu de l'écran
    glfwSetCursorPos(window, centerX, centerY);

    //Mouvements de la caméra
    float D = deltaTime * speed;
    if ( glfwGetKey( window,  GLFW_KEY_UP )   == GLFW_PRESS)
      cam += look * D;  
    if ( glfwGetKey( window,  GLFW_KEY_DOWN)  == GLFW_PRESS)
      cam -= look * D; 
    if ( glfwGetKey( window,  GLFW_KEY_RIGHT) == GLFW_PRESS)
      cam += right     * D; 
    if ( glfwGetKey( window,  GLFW_KEY_LEFT)  == GLFW_PRESS)
      cam -= right     * D; 
  }
}

void set_render_type(GLFWwindow* window){
  if( !wireframe){
    glEnable(      GL_DEPTH_TEST);
    glDepthFunc(   GL_LESS);
    glPolygonMode( GL_FRONT, GL_FILL);
    glEnable(      GL_CULL_FACE);
    //glDisable(     GL_POLYGON_OFFSET_LINE);
  }
  if( wireframe){
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
    glDisable(     GL_DEPTH_TEST);
    glDisable(     GL_CULL_FACE);
    glDepthFunc(   GL_LESS);
    glFrontFace(GL_CW);
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(2.0f);
  }
  if (render_mode == 2)
    CGL_RENDER = GL_POINTS;
  else
    CGL_RENDER = GL_TRIANGLES;
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

void screenshot(){
  BYTE* pixels = new BYTE[ 3 * width * height];
  glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
  // Convert to FreeImage format & save to file
  FIBITMAP* image = FreeImage_ConvertFromRawBits(pixels, width, height, 3 * width, 24, 0x0000FF, 0xFF0000, 0x00FF00, false);
  FreeImage_Save(FIF_BMP, image, "test.bmp", 0);
  // Free resources
  FreeImage_Unload(image);
  delete [] pixels;
  cout << "Screenshot done!" << endl;
}
