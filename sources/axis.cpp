#include "axis.h"

#include <stdlib.h>
#include <iostream>
#include <string>

#include "context.h"
#include "shader.h"


AXIS::AXIS()
{
  dash_size = 0.1;
  step = 0.1;
  gridSize = 0.5;
  axS = 0.2;
  //Grid Initialization
  for(float x = -gridSize ; x <= gridSize ; x+=step){
    for(float z = -gridSize ; z <= gridSize ; z+=step){
      GR.push_back(glm::vec3(x-dash_size/2., 0., z));
      GR.push_back(glm::vec3(x+dash_size/2., 0., z));
      GR.push_back(glm::vec3(x, 0., z-dash_size/2.));
      GR.push_back(glm::vec3(x, 0., z+dash_size/2.));
    }
  }
  //Axises
  AX.push_back(glm::vec3(-axS, 0., 0.));
  AX.push_back(glm::vec3(axS, 0., 0.));
  AX.push_back(glm::vec3(0., -axS, 0.));
  AX.push_back(glm::vec3(0., axS, 0.));
  AX.push_back(glm::vec3(0., 0., -axS));
  AX.push_back(glm::vec3(0., 0., axS));
  //Colors
  CO.push_back(glm::vec3(1.,0.,0.));
  CO.push_back(glm::vec3(0.,1.,0.));
  CO.push_back(glm::vec3(0.,0.,1.));
}

void AXIS::display(glm::mat4 MVP){
  //Parameters
  glClearDepth(1.0f);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUseProgram(0);

  // grid
  glLineWidth(1.0);
  glBegin(GL_LINES);
  glColor3f(0.5, 0.5, 0.5);
  for(int i = 0 ; i < GR.size() ; i++){
    //GR[i] = glm::vec3( MVP * glm::vec4(GR[i], 0.));
    glVertex3v(glm::vec3( MVP * glm::vec4(GR[i], 0.)));
  }
  glEnd();

  // axes
  glLineWidth(2.0);
  glBegin(GL_LINES);
  for(int i = 0 ; i < 3 ; i++){
    glColor3f(CO[i].x, CO[i].y, CO[i].z);
    glVertex3v(glm::vec3( MVP * glm::vec4(AX[2*i], 0.)));
    glVertex3v(glm::vec3( MVP * glm::vec4(AX[2*i+1], 0.)));
  }
  glEnd();
  //glLineWidth(1.0);
}

void glVertex3v(glm::vec3 v){
  glVertex3f(v.x, v.y, v.z);
}
