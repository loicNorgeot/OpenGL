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

using namespace std;

int    render_mode    = 0;
int    useColor       = true;
bool   wireframe      = false;
GLenum CGL_RENDER     = GL_POINTS;
int    width          = 1024;
int    height         = 768;
bool   UV             = true;
string RENDER         = "";

void CONTROLS::set_view( GLFWwindow* window){

  ////////////////////////////////////////////////////////////////////////////////////////:
  //Static mode
  if( !FLYINGMODE){

    //Parameters
    glfwSetInputMode( window, GLFW_CURSOR,      GLFW_CURSOR_NORMAL);
    glfwSetInputMode( window, GLFW_STICKY_KEYS, GL_TRUE);

    //Cursor data
    double xpos=0, ypos=0;
    glfwGetCursorPos( window, &xpos, &ypos);

    //Cam position modification
    if(rotating){
      if(enterRotating){
	centerX = xpos; 
	centerY = ypos;
	enterRotating=false;
      }
      
      float sensibility = 15.0f;
      //Vertical axis rotation
      glm::quat quaternionY = glm::angleAxis(float(sensibility * deltaTime * (centerX - xpos)), 
					     glm::vec3(0.0f, 1.0f, 0.0f));
      //"Right axis rotation"
      glm::quat quaternionX = glm::angleAxis(float(sensibility * deltaTime * (centerY - ypos)), 
					     glm::cross(-cam, glm::vec3(0.0f, 1.0f, 0.0f)));
      centerX = xpos; 
      centerY = ypos;

      //Quaternion application
      glm::mat4 rotationY   = glm::toMat4(quaternionY);
      glm::mat4 rotationX   = glm::toMat4(quaternionX);
      cam = glm::vec3( rotationY * rotationX * glm::vec4(cam,1));
    }

    //Camera direction update
    look = -cam;  
  }

  ////////////////////////////////////////////////////////////////////////////////////////:
  //Mode FLYING
  if(FLYINGMODE){

    //Global parameters
    glfwSetInputMode( window, GLFW_CURSOR,      GLFW_CURSOR_DISABLED);
    glfwSetInputMode( window, GLFW_STICKY_KEYS, GL_FALSE);
    
    //Cursor data
    double xpos=0, ypos=0;
    glfwGetCursorPos( window, &xpos, &ypos);

    //Upon enter
    if( ENTERFLYMODE){
      centerX      = xpos;
      centerY      = ypos;
      ENTERFLYMODE = false;
    }
    
    //Flying mode tweaking
    glm::quat horizontalQuat = glm::angleAxis(mouseSpeed * 1.0f * float(centerX - xpos),
					   glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 horizontalRot   = glm::toMat4(horizontalQuat);
    glm::vec3 right = glm::cross(look, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat verticalQuat = glm::angleAxis(mouseSpeed * 1.0f * float(centerY - ypos), right);
    glm::mat4 verticalRot   = glm::toMat4(verticalQuat);

    //Direction update
    look = glm::vec3( verticalRot * horizontalRot * glm::vec4(look,1.0f) );

    //Cursor back to the middle of screen
    glfwSetCursorPos(window, centerX, centerY);

    //Camera movements
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

void CONTROLS::set_render_type(GLFWwindow* window){
  if( !wireframe){
    glEnable(      GL_DEPTH_TEST);
    glDepthFunc(   GL_LESS);
    glPolygonMode( GL_FRONT, GL_FILL);
    glEnable(      GL_CULL_FACE);
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
  //Time computation
  static double lastTime    = glfwGetTime();
  double        currentTime = glfwGetTime(); 
  deltaTime                 = float( currentTime - lastTime);
  //Main actions
  set_view(        window);
  set_render_type( window);
  //Time update
  lastTime = currentTime;
  //Callbacks definitions
  glfwSetScrollCallback(window, scroll_callback);
  glfwSetMouseButtonCallback( window, mouse_button_callback);
  glfwSetKeyCallback(window, key_callback);
}

void screenshot(){
  BYTE* pixels = new BYTE[ 3 * width * height];
  glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
  FIBITMAP* image = FreeImage_ConvertFromRawBits(pixels, width, height, 3 * width, 24, 0x0000FF, 0xFF0000, 0x00FF00, false);
  FreeImage_Save(FIF_BMP, image, "test.bmp", 0);
  FreeImage_Unload(image);
  delete [] pixels;
  cout << "Screenshot done!" << endl;
}



///////////////////////////////////////////////////////////////////////////////
// CALLBACK FUNCTIONS

void mouse_button_callback( GLFWwindow* window,
			    int button,
			    int action,
			    int mods){
  //Displacement speed in FLYING MODE
  if ( button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    speed = 0.05f;
  if ( button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
    speed = 0.5f;

  //Mesh rotation in static mode
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
  //Zoom in static mode
  if( !FLYINGMODE){
    float D = 0.05f;
    cam -= cam * D * float(y);  
  }
  //Vertical movement in FLYING MODE
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
  
  //C to toogle color
  if ( key == GLFW_KEY_C && action == GLFW_PRESS){
      useColor=!useColor;
    cout << "Use color: " << useColor << endl;
  }
  
  //Z for wireframe
  if ( scancode == 25 && action == GLFW_PRESS){
    render_mode++;
    if(render_mode==3)
      render_mode=0;
    cout << "Render mode: " << render_mode << endl;
  }

  //F to toogle FLYING MODE
  if ( key == GLFW_KEY_F && action == GLFW_PRESS){
    if( !FLYINGMODE)
      ENTERFLYMODE=true;
    FLYINGMODE = !FLYINGMODE;
    cout << "Fly Mode: " << FLYINGMODE << endl;
  }

  //Touche S pour screenshot
  if ( key == GLFW_KEY_S && action == GLFW_PRESS)
    screenshot();

  //Numerical keypad for special orientations
  if ( key == GLFW_KEY_KP_3 && action == GLFW_PRESS){//Side
    cam = glm::vec3(-2,0,0);
    look = -cam;}
  if ( key == GLFW_KEY_KP_1 && action == GLFW_PRESS){//Face
    cam = glm::vec3(0,0,-2);
    look = -cam;}
  if ( key == GLFW_KEY_KP_7 && action == GLFW_PRESS){//Up
    cam = glm::vec3(0,2,0.1);
    look = -cam;}
}
