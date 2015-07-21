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

void dosthg(GLFWwindow* window);
void dosthg(GLFWwindow* window){
  static double lastTime = glfwGetTime();
  double currentTime = glfwGetTime(); 
  float deltaTime = float(currentTime - lastTime);

  double xpos=0, ypos=0;
  glfwGetCursorPos(window, &xpos, &ypos);
  glfwSetCursorPos(window, 1024/2, 768/2);
  hAngle += mouseSpeed * deltaTime * float(1024/2 - xpos);
  vAngle += mouseSpeed * deltaTime * float(768/2  - ypos);
  glm::vec3 direction(cos(vAngle) * sin(hAngle), 
		      sin(vAngle), 
		      cos(vAngle) * cos(hAngle));
  glm::vec3 right = glm::vec3(sin(hAngle - 3.14f/2.0f), 
			      0, 
			      cos(hAngle - 3.14f/2.0f));
  glm::vec3 up = glm::cross( right, direction);

  if (glfwGetKey(window,  GLFW_KEY_F ) == GLFW_PRESS){
    FLYINGMODE = !FLYINGMODE;
    cout << "Toogle Flying Mode: " << FLYINGMODE << endl;
  }

  if(FLYINGMODE){
    // Aller vers l'avant
    if (glfwGetKey(window,  GLFW_KEY_UP ) == GLFW_PRESS){ 
      cam += direction * deltaTime * speed; 
    } 
    // Aller vers l'arrière
    if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){ 
      cam -= direction * deltaTime * speed; 
    } 
    // Pas à droite
    if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS){ 
      cam += right * deltaTime * speed; 
    } 
    // Pas à gauche
    if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS){ 
      cam -= right * deltaTime * speed; 
    }
  }
  look = cam + direction;
  lastTime=currentTime;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    cout << "toto" << endl;
}
void scroll_callback(GLFWwindow* window, double x, double y){
  if(!FLYINGMODE){
    FOV+=y * (-2); 
  }
}

glm::mat4 CONTROLS::update_MVP(){
  glm::mat4 Projection = glm::perspective(FOV,
					  4.0f / 3.0f,
					  0.1f,
					  100.0f); 
  glm::mat4 View = glm::lookAt(cam,
			       look,
			       up);
  glm::mat4 Model = glm::mat4(1.0f);
  glm::mat4 MVP = Projection * View * Model;
  return MVP;
}

void CONTROLS::listen(GLFWwindow* window){
  glfwSetScrollCallback(window, scroll_callback);
  glfwSetMouseButtonCallback(window, mouse_button_callback);
  dosthg(window);
}
