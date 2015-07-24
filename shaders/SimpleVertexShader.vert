#version 130

in vec3 vertexPosition_modelspace;
in vec3 vertexColor;
out vec3 fragmentColor; 

uniform float change;
uniform mat4 MVP;

void main(){
  //Position du sommet, dans l'espace de découpe : MVP * position
  gl_Position = MVP * vec4(vertexPosition_modelspace,1);
  if(change==0.0f)
    fragmentColor=vec3(0.00f, 0.00f, 0.00f);
  else if(change==1.0f)
    fragmentColor=vec3(0.8f, 0.8f, 0.8f);
  else if(change==2.0f)
    fragmentColor=vec3(0.6f, 0.6f, 0.6f);
  //fragmentColor = vertexColor;
}
