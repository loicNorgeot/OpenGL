#version 130

//layout(location = 0) Non compatible avec glsl>1.3
in vec3 vertexPosition_modelspace;
in vec3 vertexColor;
out vec3 fragmentColor; 

uniform mat4 MVP;

void main(){
  //Position du sommet, dans l'espace de découpe : MVP * position
  gl_Position = MVP * vec4(vertexPosition_modelspace,1);
  fragmentColor = vertexColor; 
}
