#ifndef AXIS_H
#define AXIS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <glm/glm.hpp>

#include <vector>

class AXIS
{
  public:
    GLuint axisBuffer;
    std::vector<glm::vec3> GR;
    std::vector<glm::vec3> AX;
    std::vector<glm::vec3> CO;
    float dash_size;
    float step;
    float gridSize;
    float axS;
    AXIS();
    void display(glm::mat4 MVP);

    friend void glVertex3v(glm::vec3 v);
};
void glVertex3v(glm::vec3 v);
#endif // AXIS_H
