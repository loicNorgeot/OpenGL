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

//#include "assimpMesh.h"

using namespace std;

int main(){
  SHADER   shader;
  CONTEXT  context;
  TEXTURE  texture;
  Mesh     mesh;

  //Render type
  RENDER = "MESH_SOL";

  //Input paths
  string assetPath = "/home/loic/OPENGL/assets/";
  string shaderPath = "/home/loic/OPENGL/shaders/";
  string meshFile  = assetPath + "FauduetBone.o1.mesh";
  string solFile   = assetPath + "toto.sol";
  string plyFile   = assetPath + "c_nardoni.ply";
  //string plyFileT  = assetPath + "3260.ply";
  string plyFileT  = "/home/loic/Ply/F_3354.ply";
  string imgFile   = assetPath + "c_nardoni_2048.jpg";
  FIBITMAP *dib1   = NULL;

  cout << "toto" << endl;

  //Parameters according to the render type
  if((RENDER=="MESH_SOL") || (RENDER=="MESH") || (RENDER=="PLY_COLORS"))
    UV = false;
  else if(RENDER=="PLY_UV")
    UV = true;

  if(RENDER=="MESH_SOL"){
    Mesh M(RENDER, meshFile, solFile, "");
    mesh = M;
  }
  else if(RENDER=="MESH"){
    Mesh M(RENDER, meshFile, "", "");
    mesh = M;
  }
  else if(RENDER=="PLY_UV"){
    Mesh M(RENDER, plyFile, "", imgFile);
    mesh = M;
  }
  else if(RENDER=="PLY_COLORS"){
    Mesh M(RENDER, plyFileT, "", "");
    mesh = M;
  }

  //Buffer array
  float *vert_data   = &mesh.verts[0];
  float *uv_data     = &mesh.uv[0];
  float *normal_data = &mesh.normals[0];
  float *color_data  = &mesh.colors[0];
  int   *indice_data = &mesh.indices[0];

  int numIndices = mesh.indices.size();
  int numVerts   = mesh.verts.size()/3;

  //Texture loading
  if(UV)
    texture.loadImage(imgFile);

  context.init(width, height, "TESTS OPENGL", 3, 3);
  context.nbVertices    = numVerts;
  context.vertexbuffer  = context.GL_array_buffer( vert_data, 3);

  /////////////////////////////
  //.MESH
  if((RENDER == "MESH") || (RENDER == "MESH_SOL")){
    if(RENDER=="MESH_SOL"){
      context.colorbuffer   = context.GL_array_buffer( color_data, 1);
      shader.load(shaderPath + "sol_shader.vert", shaderPath + "sol_shader.frag");
    }
    else if (RENDER == "MESH")
      shader.load(shaderPath + "uni_shader.vert", shaderPath + "uni_shader.frag");
    context.nbIndices     = numIndices;
    context.normalbuffer  = context.GL_array_buffer( normal_data, 3);
    context.indicesbuffer = context.GL_index_buffer( indice_data);
  }

  /////////////////////////////
  //.PLY without UV texture
  else if(RENDER=="PLY_COLORS"){
    context.nbIndices     = numIndices;
    context.indicesbuffer = context.GL_index_buffer( indice_data);
    context.colorbuffer   = context.GL_array_buffer( color_data, 3);
    shader.load(shaderPath + "col_shader.vert", shaderPath + "col_shader.frag");
  }

  /////////////////////////////
  //.PLY with UV texture
  else if(RENDER=="PLY_UV"){
    context.uvbuffer      = context.GL_array_buffer( uv_data, 2);
    context.colorbuffer   = context.GL_array_buffer( color_data, 2);
    shader.load(shaderPath + "uv_shader.vert", shaderPath + "uv_shader.frag");
  }

  context.programID = shader.mProgramID;
  //Matrix link
  context.MatrixID      = glGetUniformLocation(context.programID, "MVP");

  //Texture loading
  if(UV)
    texture.loadTexture();

  //Main loop
  context.loop();

  //Freeing ressources
  if(UV)
    texture.unloadTexture();
  glfwDestroyWindow(context.window);

  return 1;
}
