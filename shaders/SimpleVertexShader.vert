#version 130

in vec3 vertexPosition_modelspace;
in vec3 vertexColor;
in vec3 normals;
out vec3 fragmentColor; 

uniform float change;
uniform mat4 MVP;

void main(){
  //Position du sommet, dans l'espace de découpe : MVP * position
  gl_Position = MVP * vec4(vertexPosition_modelspace,1);
  
  if(change==0.0f)
    fragmentColor=vec3(0.08f, 0.08f, 0.08f);
  else if(change==1.0f)
    fragmentColor=vec3(1.0f, 0.55f, 0.0f);
  else if(change==2.0f)
    fragmentColor=vec3(1.0f, 1.0f, 1.0f);
  
  //fragmentColor = normals;
  //fragmentColor = vertexColor;
}
