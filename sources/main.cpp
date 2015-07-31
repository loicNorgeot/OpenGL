#include <stdlib.h>
#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h> 

#include "shader.h"
#include "controls.h"
#include "context.h"
#include "loader.h"
#include "texture.h"

using namespace std;

int main(){
  //Render type
  RENDER = "PLY_UV";

  //Input paths
  string assetPath = "assets/";
  string meshFile  = assetPath + "FauduetBone.o1.mesh";
  string solFile   = assetPath + "toto.sol";
  string plyFile   = assetPath + "c_nardoni.ply";
  string plyFileT  = assetPath + "3260.ply";
  string imgFile   = assetPath + "c_nardoni_2048.jpg";
  FIBITMAP *dib1   = NULL;

  //Parameters according to the render type
  Mesh mesh;
  if(RENDER=="MESH_SOL"){
    UV = false;
    Mesh M(RENDER, meshFile, solFile, "");
    mesh = M;
  }
  else if(RENDER=="MESH"){
    UV = false;
    Mesh M(RENDER, meshFile, "", "");
    mesh = M;
  }
  else if(RENDER=="PLY_UV"){
    UV = true;
    Mesh M(RENDER, plyFile, "", imgFile);
    mesh = M;
  }
  else if(RENDER=="PLY_COLORS"){
    UV = false;
    Mesh M(RENDER, plyFileT, "", "");
    mesh = M;
  }

  //Texture loading
  if(UV)
    dib1 = loadImage(imgFile);

  //Buffer array
  float *vert_data   = &mesh.verts[0];
  float *uv_data     = &mesh.uv[0];
  float *normal_data = &mesh.normals[0];
  float *color_data  = &mesh.colors[0];
  int   *indice_data = &mesh.indices[0];


  //Initialization
  CONTEXT context;
  context.init(width, height, "TESTS OPENGL", 3, 3);
  

  //Buffers and context specifications
  context.nbVertices    = mesh.verts.size()/3;
  context.vertexbuffer  = context.GL_array_buffer( vert_data, 3);

  /////////////////////////////
  //.MESH
  if((RENDER == "MESH") || (RENDER == "MESH_SOL")){
    if(RENDER=="MESH_SOL"){
      context.colorbuffer   = context.GL_array_buffer( color_data, 1);
      context.programID     = LoadShaders("shaders/sol_shader.vert",
					  "shaders/sol_shader.frag");
    }
    else if (RENDER == "MESH")
      context.programID     = LoadShaders("shaders/uni_shader.vert",
					  "shaders/uni_shader.frag");
    context.nbIndices     = mesh.indices.size();
    context.normalbuffer  = context.GL_array_buffer( normal_data, 3);
    context.indicesbuffer = context.GL_index_buffer( indice_data);
    
  }
  
  /////////////////////////////
  //.PLY without UV texture
  else if(RENDER=="PLY_COLORS"){
    context.programID     = LoadShaders("shaders/col_shader.vert",
					"shaders/col_shader.frag");
    context.nbIndices     = mesh.indices.size();
    context.indicesbuffer = context.GL_index_buffer( indice_data);
    context.colorbuffer   = context.GL_array_buffer( color_data, 3);
  }
    
  /////////////////////////////
  //.PLY with UV texture
  else if(RENDER=="PLY_UV"){
    context.programID     = LoadShaders("shaders/uv_shader.vert",
					"shaders/uv_shader.frag");
    context.uvbuffer      = context.GL_array_buffer( uv_data, 2);
    context.colorbuffer   = context.GL_array_buffer( color_data, 2);
  }


  //Matrix link
  context.MatrixID      = glGetUniformLocation(context.programID, "MVP"); 

  //Texture loading
  if(UV)
    loadTexture(dib1);

  //Main loop
  context.loop();

  //Freeing ressources
  if(UV)
    FreeImage_Unload(dib1);
  glfwDestroyWindow(context.window);

  return 1;
}
