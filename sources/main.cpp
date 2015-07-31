#include <stdio.h> 
#include <stdlib.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <FreeImage.h>

#include "shader.h"
#include "controls.h"
#include "context.h"

#include "loader.h"

using namespace std;
using namespace glm;

FIBITMAP  *loadImage(string filename);
GLuint loadTexture (FIBITMAP  * dib1);

int main(){
  //Chemins des fichiers d'entrée
  string assetPath = "assets/";
  string meshFile  = assetPath + "FauduetBone.o1.mesh";//NULL;
  string solFile   = assetPath + "toto.sol";//NULL;
  string plyFile   = assetPath + "c_nardoni.ply";//NULL;
  string plyFileT  = assetPath + "3260.ply";
  string imgFile   = assetPath + "c_nardoni_2048.jpg";//NULL;
  FIBITMAP *dib1   = NULL;

  //Paramètres en fonction du type souhaité
  RENDER = "PLY_COLORS";
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

  //Chargement de la texture
  if(UV)
    dib1 = loadImage(imgFile);

  //Création des tableaux de buffer
  float *vert_data   = &mesh.verts[0];
  float *uv_data     = &mesh.uv[0];
  float *normal_data = &mesh.normals[0];
  float *color_data  = &mesh.colors[0];
  int   *indice_data = &mesh.indices[0];


  //Initialisation
  CONTEXT context;
  context.init(width, height, "TESTS OPENGL", 3, 3);
  

  //Création des buffers et spécifications du contexte
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
    //cout << mesh.colors.size() << endl;
    //cout << context.nbVertices << endl;
    //cout << context.nbIndices << endl;
    //for(int i = 0 ; i < mesh.colors.size() ; i+=mesh.colors.size()/10)
    //  cout << color_data[i+0] << " " 
    //   << color_data[i+1] << " " 
    //	   << color_data[i+2] << endl;
  }
    
  /////////////////////////////
  //.PLY with UV texture
  else if(RENDER=="PLY_UV"){
    context.programID     = LoadShaders("shaders/uv_shader.vert",
					"shaders/uv_shader.frag");
    context.uvbuffer      = context.GL_array_buffer( uv_data, 2);
    context.colorbuffer   = context.GL_array_buffer( color_data, 2);
  }


  //Linkage de la matrice
  context.MatrixID      = glGetUniformLocation(context.programID, "MVP"); 

  //Chargement de la texture
  if(UV)
    loadTexture(dib1);

  //Boucle principale
  context.loop();

  //Nettoyage
  if(UV)
    FreeImage_Unload(dib1);
  glfwDestroyWindow(context.window);

  return 1;
}





////////////////////////////////////////////////////////////////////////////
//IMAGE AND TEXTURE LOADING

FIBITMAP  *loadImage(string filename){
  FIBITMAP *dib1 = NULL;
  FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(filename.c_str());
  dib1 = FreeImage_Load(fif, filename.c_str(), JPEG_DEFAULT);
  if (!dib1){
    std::cerr << "Erreur ouverture d\'image" << std::endl;
    exit (0);
  } 
  return dib1;
}

GLuint loadTexture (FIBITMAP  * dib1){
  GLuint tex_id = 0;
  int x, y;
  int height, width;

  RGBQUAD rgbquad;
  FREE_IMAGE_TYPE type;
  BITMAPINFOHEADER *header;
  
  type = FreeImage_GetImageType(dib1);
  height = FreeImage_GetHeight(dib1);
  width = FreeImage_GetWidth(dib1);
  
  header = FreeImage_GetInfoHeader(dib1);

  int scanLineWidh = ((3*width)%4 == 0) ? 3*width : ((3*width)/4)*4+4;
  unsigned char * texels= (GLubyte*)calloc(height*scanLineWidh, sizeof(GLubyte));

  for (x=0 ; x<width ; x++){
    for (y=0 ; y<height; y++){  
      FreeImage_GetPixelColor(dib1,x,y,&rgbquad);
      
      texels[(y*scanLineWidh+3*x)]=((GLubyte*)&rgbquad)[2];
      texels[(y*scanLineWidh+3*x)+1]=((GLubyte*)&rgbquad)[1];
      texels[(y*scanLineWidh+3*x)+2]=((GLubyte*)&rgbquad)[0];
    }
  }
  
  glGenTextures (1, &tex_id);
  glBindTexture (GL_TEXTURE_2D, tex_id);
  
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
  glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB,
		width, height, 0, GL_RGB,
		GL_UNSIGNED_BYTE, texels);
  free(texels);
  
  return tex_id;
}














