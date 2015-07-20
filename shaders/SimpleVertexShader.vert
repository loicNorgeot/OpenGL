#version 130

//layout(location = 0) Non compatible avec glsl>1.3
in vec3 vertexPosition_modelspace;
uniform mat4 MVP;

void main(){
  //gl_Position.xyz = vertexPosition_modelspace; 
  //gl_Position.w = 1.0; 
  //Position du sommet, dans l'espace de découpe : MVP * position
  //vec4 v = vec4(vertexPosition_modelspace,1); // Vecteur 4D homogène
  gl_Position = MVP * vec4(vertexPosition_modelspace,1);

  //vec4 v4 = MVP * v;
  //gl_Position.xyz = vec3(v4[0], v4[1], v4[2]); 
}
