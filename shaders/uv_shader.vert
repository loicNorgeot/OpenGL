#version 130

in vec3 vertexPosition_modelspace;
in vec2 uv;
out vec2 UV; 

out float render;
out float useCol;

uniform mat4 MVP;
uniform float renderMode;
uniform float useColor;

void main(){
  UV = uv;
  gl_Position = MVP * vec4(vertexPosition_modelspace,1);
  
  render = renderMode;
  useCol = useColor;

}




