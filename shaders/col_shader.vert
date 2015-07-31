#version 130

in vec3 vertexPosition_modelspace;
in vec3 vertexColor;
out vec3 fragmentColor; 

uniform float renderMode;
uniform float useColor;
uniform mat4 MVP;

void main(){
  gl_Position = MVP * vec4(vertexPosition_modelspace,1);

  //Faces et wireframes
  if(renderMode==0.0f){
    if(useColor==0.0f)
      fragmentColor=vec3(1.0f, 1.0f, 1.0f);
    else
      fragmentColor= vertexColor;
  }

  //Wireframes seulement
  else if(renderMode==1.0f){
    if(useColor==0.0f)
      fragmentColor = vec3(0.3f, 0.3f, 0.3f);
    else
      fragmentColor = vertexColor;//vec3(0.9f, 0.9f, 0.9f);
  }

  //Nuages de points
  else if(renderMode==2.0f){
    if(useColor==0.0f)
      fragmentColor=vec3(1.0f, 1.0f, 1.0f);
    else 
      fragmentColor= vertexColor;
  }

}
