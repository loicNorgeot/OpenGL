#version 130

in vec3 vertexPosition_modelspace;
out vec3 fragmentColor; 

uniform float renderMode;
uniform float useColor;
uniform mat4 MVP;

void main(){
  //Position du sommet, dans l'espace de découpe : MVP * position
  gl_Position = MVP * vec4(vertexPosition_modelspace,1);

  //Faces et wireframes
  if(renderMode==0.0f){
    if(useColor==0.0f)
      fragmentColor=vec3(0.85f, 0.85f, 0.85f);
    else
      fragmentColor= vec3(0.0f, 0.0f, 1.0f);
  }

  //Wireframes seulement
  else if(renderMode==1.0f){
    if(useColor==0.0f)
      fragmentColor = vec3(0.3f, 0.3f, 0.3f);
    else
      fragmentColor = vec3(0.9f, 0.9f, 0.9f);
  }

  //Nuages de points
  else if(renderMode==2.0f){
    if(useColor==0.0f)
      fragmentColor=vec3(1.0f, 1.0f, 1.0f);
    else 
      fragmentColor= vec3(0.0f, 0.0f, 1.0f);
  }
}
