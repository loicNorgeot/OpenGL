/** 
 * \brief       Class managing the whole application
 *
 * \details     Manages the main application data, parameters and renders.
 * The window and its parameters are created and linked to OpenGL program.
 * Allows for buffer creation, manipulation, and linking with GLSL shader.
 */

#ifndef CONTEXTHEADERDEF
#define CONTEXTHEADERDEF

#include <string>
#include "controls.h"


class CONTEXT{
public:
  GLFWwindow* window;

  GLuint VertexArrayID;
  GLuint programID;
  GLuint MatrixID;

  GLuint vertexbuffer;
  GLuint colorbuffer;
  GLuint normalbuffer;
  GLuint indicesbuffer;
  GLuint uvbuffer;

  int nbVertices;
  int nbIndices;

  glm::mat4 MVP;

  CONTROLS controls;

  /** 
   * \brief       Initializes GLFW and GLEW, and creates a window and VAO
   *
   * \details     Proceeds to GLFWInit(), then creates the window   
   * and puts it in the current context.
   * GLEW is then initialized, before the definition of some basic OpenGL 
   * properties, as clear color, multisampling... etc
   * \param   sizeX       window width
   * \param   sizeY       window height
   * \param   windowName  window title
   * \param   major       OpenGL version major
   * \param   minor       OpenGL version minor
   * \return  integer representing the execution status
   */
  int init(int sizeX, int sizeY, std::string windowName, int major, int minor);


  /** 
   * \brief    Fills a buffer
   *
   * \details  Loads data from a dynamic array and binds it to a buffer 
   * to be later sent to the GLSL shader program
   * \param    p_data      data to buffer
   * \param    dimension   dimension of the data (3 for 3D vertex, 1 for scalar...)
   * \return   GLuint representing the buffer, used when linking with OpenGL shaders
   */
  GLuint GL_array_buffer(const float* p_data, int dimension);

  /** 
   * \brief    Opens an indices buffer
   *
   * \details  Loads indices data from an integer array,
   * representing the indices of faces' vertices.
   * \param    indices   1D array of indices to be sent
   * \return   GLuint representing the index buffer, used when linking with OpenGL shaders
   */
  GLuint GL_index_buffer( const int* indices);

  /** 
   * \brief    Links a buffer with a shader attribute
   *
   * \details  Links data from a buffer to a corresponding attribute
   * located in the GLSL shader program.
   * \param    buffer    index of the buffer to be sent
   * \param    index     location of the buffer, can come from glGetAttribLocation
   * \param    dimension dimension of the data
   * \param    name      name of the GLSL's variable corresponding to the data
   * \return   None
   */
  void GL_attributes(GLuint buffer, int index, int dimension, std::string name);

  /** 
   * \brief    Executes the main loop
   *
   * \details  Executed at each call, this function loads the main parameters,
   * updates the controls and position attributes, and links every buffer
   * with the corresponding attributes.
   * Depending on the render modes, the main OpenGL calls are done,
   * before freeing the ressources.
   * \return   None
   */
  void loop();
};

#endif
