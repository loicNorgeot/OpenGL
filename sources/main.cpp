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
#include "mesh.h"

#include "ply.h"

using namespace std;
using namespace glm;

FIBITMAP  *loadImage(string filename);
GLuint loadTexture (FIBITMAP  * dib1);

int main(){
  //Chemins des fichiers d'entrée
  string meshFile = "/home/loic/Blender/MESH/FauduetBone.o1.mesh";//NULL;
  string solFile  = "/home/loic/toto.sol";//NULL;
  string plyFile  = "/home/loic/Blender/PLY/c_nardoni.ply";//NULL;
  string imgFile  = "/home/loic/Blender/TEX/c_nardoni_2048.jpg";//NULL;
  FIBITMAP *dib1  = NULL;

  //Lecture d'un fichier .MESH, avec éventuellement un .sol correspondant
  std::vector<float> g_vertex, g_normal, g_solution;
  std::vector<int>   g_indices;
  if(!UV){
    mesh_read(meshFile, 
	      solFile,
	      g_vertex, 
	      g_normal, 
	      g_indices,
	      g_solution);
  }

  //Lecture d'un fichier .ply, et spécification de l'image
  std::vector<float> g_vert, g_uv;
  if(UV){
    ply_read(plyFile,
	     g_vert,
	     g_uv);
    //Chargement de la texture
    dib1 = loadImage(imgFile);
    loadTexture(dib1);
  }

  //Tableaux a transférer
  float *vert_data   = &g_vert[0];
  float *uv_data     = &g_uv[0];
  float *scalar_data = &g_solution[0];
  float *vertex_data = &g_vertex[0];
  float *normal_data = &g_normal[0];
  int   *indice_data = &g_indices[0];
  
  //Initialisation
  CONTEXT context;
  context.init(width, height, "TESTS OPENGL", 3, 3);
  
  //Création des buffers et spécifications du contexte
  if(!UV){
    context.programID     = LoadShaders("shaders/classic_shader.vert",
					"shaders/classic_shader.frag");
    context.nbVertices    = g_vertex.size()/3;
    context.nbIndices     = g_indices.size();
    context.vertexbuffer  = context.GL_array_buffer( vertex_data, 3);
    context.colorbuffer   = context.GL_array_buffer( scalar_data, 1);
    context.normalbuffer  = context.GL_array_buffer( normal_data, 3);
    context.indicesbuffer = context.GL_index_buffer( indice_data, context.nbIndices);
  }
  else if(UV){
    context.programID     = LoadShaders("shaders/uv_shader.vert",
					"shaders/uv_shader.frag");
    context.nbVertices    = g_vert.size()/3;
    context.vertexbuffer  = context.GL_array_buffer( vert_data, 3);
    context.uvbuffer      = context.GL_array_buffer( uv_data, 2);
  }

  //Linkage de la matrice
  context.MatrixID      = glGetUniformLocation(context.programID, "MVP"); 

  loadTexture(dib1);
  //Boucle principale
  context.loop();

  //Nettoyage
  if(UV)
    FreeImage_Unload(dib1);
  glfwDestroyWindow(context.window);
  //delete[] scalar_data;
  //delete[] vertex_data;
  //delete[] normal_data;
  //delete[] indice_data;
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














