#include <stdio.h> 
#include <iostream>
#include <stdlib.h>
#include <string>
#include <cstdlib>
#include <GL/glew.h>
#include <FreeImage.h>

#include "texture.h"

using namespace std;

TEXTURE::TEXTURE(){
  mImgPath = "";
  mDib1 = NULL;
  mTexID = 0;
}

int TEXTURE::loadImage(string image_path){
  mImgPath = image_path;
  FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(mImgPath.c_str());
  mDib1 = FreeImage_Load(fif, mImgPath.c_str(), JPEG_DEFAULT);
  if (!mDib1){
    std::cerr << "Erreur ouverture d\'image" << std::endl;
    return 0;
  } 
  return 1;
}

int TEXTURE::loadTexture (){
  int x, y;
  int height, width;

  RGBQUAD rgbquad;
  FREE_IMAGE_TYPE type;
  BITMAPINFOHEADER *header;
  
  type = FreeImage_GetImageType(mDib1);
  height = FreeImage_GetHeight(mDib1);
  width = FreeImage_GetWidth(mDib1);
  
  header = FreeImage_GetInfoHeader(mDib1);

  int scanLineWidh = ((3*width)%4 == 0) ? 3*width : ((3*width)/4)*4+4;
  unsigned char * texels= (GLubyte*)calloc(height*scanLineWidh, sizeof(GLubyte));

  for (x=0 ; x<width ; x++){
    for (y=0 ; y<height; y++){  
      FreeImage_GetPixelColor(mDib1,x,y,&rgbquad);
      
      texels[(y*scanLineWidh+3*x)]=((GLubyte*)&rgbquad)[2];
      texels[(y*scanLineWidh+3*x)+1]=((GLubyte*)&rgbquad)[1];
      texels[(y*scanLineWidh+3*x)+2]=((GLubyte*)&rgbquad)[0];
    }
  }
  
  glGenTextures (1, &mTexID);
  glBindTexture (GL_TEXTURE_2D, mTexID);
  
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  
  glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB,
		width, height, 0, GL_RGB,
		GL_UNSIGNED_BYTE, texels);
  free(texels);
  return 1;
}

int TEXTURE::unloadTexture(){
  FreeImage_Unload(mDib1);
  return 1;
}
