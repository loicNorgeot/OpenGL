#ifndef TEXTUREHEADERDEF
#define TEXTUREHEADERDEF

#include <string>
#include <GL/glew.h>
#include <FreeImage.h>

class TEXTURE{
 public:
  std::string mImgPath;
  FIBITMAP *mDib1;
  GLuint mTexID;
  
  TEXTURE();
  int loadImage(std::string image_path);
  int loadTexture();
  int unloadTexture();
};

//FIBITMAP  *loadImage(std::string filename);
//GLuint loadTexture (FIBITMAP  * dib1);

#endif
