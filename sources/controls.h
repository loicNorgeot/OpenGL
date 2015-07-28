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

#include <FreeImage.h>

//Variables globales à initialiser dans uniquement un fichier!!!
#include "globals.h"

static float FOV         = 45.0;
static glm::vec3 cam     = glm::vec3( 0, 0, 2.0);
static glm::vec3 look    = glm::vec3( 0.0, 0.0, 0.0);
static glm::vec3 up      = glm::vec3( 0.0, 1.0, 0.0);
static float speed       = 0.5f;
static float mouseSpeed  = 0.2f; 

static bool FLYINGMODE   = false;
static bool ENTERFLYMODE = false;

static bool rotating     = false;
static bool enterRotating= false;

static float deltaTime   = 0;

static int centerX       = 1024/2;
static int centerY       = 768/2;

void scroll_callback(       GLFWwindow* window, double x,   double y);
void mouse_button_callback( GLFWwindow* window, int button, int action,   int mods);
void key_callback(          GLFWwindow* window, int key,    int scancode, int action, int mods);

void GUI(             GLFWwindow* window);
void set_render_type( GLFWwindow* window);
void set_view(        GLFWwindow* window);
void screenshot();

class CONTROLS{
public:
  glm::mat4 update_MVP();
  void      listen(   GLFWwindow* window);
};

#endif


