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
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
  
  //Print des caractéristiques de l'appui
  cout << key << "\t " << scancode << "\t" << action << "\t" << mods << endl;
  
  //Touche Z pour mode wireframe
  if (scancode == 25 && action == GLFW_PRESS){
    wireframe++;
    if(wireframe==3){wireframe=0;}
    cout << "Wireframe mode = " << wireframe << endl;
    if(wireframe==0){
      glPolygonMode(GL_FRONT, GL_FILL);
      glEnable(GL_CULL_FACE);
    }
    if(wireframe==1){
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glEnable(GL_CULL_FACE);
    }
    if(wireframe==2){
      glPolygonMode(GL_FRONT, GL_LINE);
      glDisable(GL_DEPTH_TEST);
      glDisable(GL_CULL_FACE);
    }
  }

  //Touche F pour FLY mode
  if (key == GLFW_KEY_F && action == GLFW_PRESS){
    FLYINGMODE = !FLYINGMODE;
    cout << "Fly Mode: " << FLYINGMODE << endl;
  }
}

void displacement(GLFWwindow* window){
  //Calcul du deltaTime
  static double lastTime = glfwGetTime();
  double currentTime = glfwGetTime(); 
  float deltaTime = float(currentTime - lastTime);

  //Ajustements du FlyingMode
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
  look = cam + direction;

  if(!FLYINGMODE){
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  }

  if(FLYINGMODE){
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_FALSE);
    if (glfwGetKey(window,  GLFW_KEY_UP ) == GLFW_PRESS){
      cam += direction * deltaTime * speed; 
    } 
    if (glfwGetKey(window,  GLFW_KEY_DOWN) == GLFW_PRESS){ 
      cam -= direction * deltaTime * speed; 
    } 
    if (glfwGetKey(window,  GLFW_KEY_RIGHT) == GLFW_PRESS){ 
      cam += right * deltaTime * speed; 
    } 
    if (glfwGetKey(window,  GLFW_KEY_LEFT) == GLFW_PRESS){ 
      cam -= right * deltaTime * speed; 
    }
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
  }
  
  //Actualisation du temps
  lastTime=currentTime;
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
  glfwSetKeyCallback(window, key_callback);
  displacement(window);
  //dosthg(window);
}
