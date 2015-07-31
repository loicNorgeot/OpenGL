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


/** 
 * \brief       Class managing the user interface and corresponding actions
 *
 * \details     Creates the different callbacks with material inputs,
 * and links the relevant ones with corresponding actions (mesh rotating, render mode...).
 * Allows for an easy update of the application parameters.
 */
class CONTROLS
{

public:

  /** 
   * \brief      Sets the camera according to the user choices
   *
   * \details    Sets the camera position and orientation 
   * depending on the current mode: FLYING or STATIC
   * Quaternions are used to rotate the view, 
   * and three main vectors are used:\n
   * - cam     defines the camera position\n
   * - look    defines the camera orientation\n
   * - up      defines the world's vertical\n
   * \param      window     current window
   * \return     None
   */
  void set_view(              GLFWwindow* window);

  /** 
   * \brief      Sets the render mode according to the user choices
   *
   * \details    Sets the OpenGL parameters corresponding to the render type:\n
   * - wireframe or full\n
   * - triangle representation or points cloud
   * \param      window     current window
   * \return     None
   */
  void set_render_type(       GLFWwindow* window);

  /** 
   * \brief      Updates the Model View Projection Matrix
   *
   * \details    Updates the Model View Projection Matrix,
   * according to the modifications of cam, look and up vectors.
   * \return     Model View Projection Matrix, later sent to GLSL shader
   */
  glm::mat4 update_MVP();

  /** 
   * \brief      Links the control interface
   *
   * \details    Links the main application with the control interface,
   * and acts on views, render modes and inputs.
   * This method is later called to listen to user actions.
   * \return     None
   */
  void      listen(           GLFWwindow* window);  
};



/** 
 * \relates   CONTROLS
 * \brief     Links the mouse wheel actions
 *
 * \details   Links the mouse wheel to zoom in static mode,
 * and to vertical displacement in FLYING MODE
 * \param     window     current window
 * \param     x          horizontal scroll
 * \param     y          vertical scroll
 * \return    None
 */
void scroll_callback(       GLFWwindow* window, double x,   double y);

/** 
 * \relates  CONTROLS
 * \brief    Links the mouse actions
 *
 * \details  Links the right button to speed in flying mode,
 * and the left button to rotation in static mode.
 * \param    window     current window
 * \param    button     active button
 * \param    action     button action
 * \param    mods       special modifications
 * \return   None
 */
void mouse_button_callback( GLFWwindow* window, int button, int action,   int mods);

/** 
 * \relates  CONTROLS
 * \brief    Links the keyboard actions
 *
 * \details  Links the different keys to actions:\n
 * - C    :  toogle color\n
 * - Z    :  switch render mode (full, wireframe, points)\n
 * - F    :  toogle FLYING MODE\n
 * - S    :  records a screenshot\n
 * - NP1  :  front wiew\n
 * - NP3  :  side wiew\n
 * - NP7  :  up wiew
 * \param    window     current window
 * \param    key        name of the key
 * \param    scancode   integer representation of the key
 * \param    action     button action
 * \param    mods       special modifications
 * \return   None
 */
void key_callback(          GLFWwindow* window, int key,    int scancode, int action, int mods);

/** 
 * \relates  CONTROLS
 * \brief    Records a screenshot
 *
 * \details  Records a screenshot with the S key
 * \param    window     current window
 * \return   None
 */
void screenshot();


#endif


