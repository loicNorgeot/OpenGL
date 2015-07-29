#version 130

in float render;
in float useCol;
in vec2 UV;
out vec3 color;

uniform sampler2D textureSampler;

void main(){
  if(render==0.0f)
    if(useCol==1.0f)
      color = texture( textureSampler, UV ).rgb;
    else
      color=vec3(0.9f, 0.9f, 0.9f);

  if(render==1.0f)
    if(useCol==1.0f)
      color = texture( textureSampler, UV ).rgb;
    else
      color=vec3(0.2f, 0.2f, 0.2f);

  if(render==2.0f)
    if(useCol==1.0f)
      color = texture( textureSampler, UV ).rgb;
    else
      color=vec3(1.0f, 1.0f, 1.0f);

  

}
