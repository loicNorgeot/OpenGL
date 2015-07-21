#version 130

in vec3 fragmentColor;
out vec3 color;

void main(){
  //color = vec3(1,1,0);
    // Output color = color spécifié dans le vertex shader
    // interpolé entre les trois sommets alentours
    color = fragmentColor; 
}
