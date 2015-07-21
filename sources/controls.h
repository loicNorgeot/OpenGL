#ifndef CONTROLSHEADERDEF
#define CONTROLSHEADERDEF

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

static float FOV        = 45.0;
static bool  FLYINGMODE = false;
static glm::vec3 cam    = glm::vec3(4.0, 1.5, 3.0);
static glm::vec3 look   = glm::vec3(0.0, 0.0, 0.0);
static glm::vec3 up     = glm::vec3(0.0, 1.0, 0.0);
static float hAngle     = 3.14f;
static float vAngle     = 0.0f;
static float speed      = 8.0f;
static float mouseSpeed = 0.2f; 


void scroll_callback(GLFWwindow* window,double x,double y);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

class CONTROLS{
public:
  glm::mat4 update_MVP();
  void listen(GLFWwindow* window);
};

#endif


