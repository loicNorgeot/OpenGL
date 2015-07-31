#ifndef TEXTUREHEADERDEF
#define TEXTUREHEADERDEF

#include <string>
#include <GL/glew.h>
#include <FreeImage.h>

FIBITMAP  *loadImage(std::string filename);
GLuint loadTexture (FIBITMAP  * dib1);

#endif
