#version 130

in vec3 vertexPosition_modelspace;
in float vertexColor;
in vec3 normals;
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
    else{
      float a = (1-vertexColor)/0.2;
      int X   = int(a);
      float Y = (a-X);
      float r,g,b;
      switch(X)
	{
	case 0: r=1.0;    g=Y;   b=0;   break;
	case 1: r=1.0-Y;  g=1;   b=0;   break;
	case 2: r=0;      g=1;   b=Y;   break;
	case 3: r=0;      g=1-Y; b=1;   break;
	case 4: r=0;      g=0;   b=1;   break;
	}
      fragmentColor= vec3(r,g,b);
    }
  }

  //Wireframes seulement
  else if(renderMode==1.0f){
    if(useColor==1.0f)
      fragmentColor = vec3(0.0f, 0.0f, 0.0f);
    else if(useColor==0.0f)
      fragmentColor = vec3(0.3f, 0.3f, 0.3f);
  }

  //Nuages de points
  else if(renderMode==2.0f){
    if(useColor==0.0f)
      fragmentColor=vec3(1.0f, 1.0f, 1.0f);
    else if(useColor==1.0f){
      float a = (1-vertexColor)/0.2;
      int X   = int(a);
      float Y = (a-X);
      float r,g,b;
      switch(X)
	{
	case 0: r=1.0;    g=Y;   b=0;   break;
	case 1: r=1.0-Y;  g=1;   b=0;   break;
	case 2: r=0;      g=1;   b=Y;   break;
	case 3: r=0;      g=1-Y; b=1;   break;
	case 4: r=0;      g=0;   b=1;   break;
	}
      fragmentColor= vec3(r,g,b);
    }
  }

}
