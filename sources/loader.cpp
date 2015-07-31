#include <string.h>
#include <vector>
#include <glm/glm.hpp>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdio.h>

#include "loader.h"

using namespace std;

const int         MAX_CHARS_PER_LINE  = 512;
const int         MAX_TOKENS_PER_LINE = 20;
const char* const DELIMITER           = " ";

static bool   readNum[] = {false,      false,       false,     false};
static bool   readElt[] = {false,      false,       false,     false};
static int    eltInc[]  = {0,          0,           0,         0};
static int    sizes[]   = {0,          0,           0,         0};

static int   lineNumber = 0;
static bool  firstFloat = true;

Mesh& Mesh::operator=(const Mesh& otherMesh){
  type    = otherMesh.type;
  verts   = otherMesh.verts;
  colors  = otherMesh.colors;
  normals = otherMesh.normals;
  indices = otherMesh.indices;
  uv      = otherMesh.uv;
  return *this;
}

Mesh::Mesh(){
  type    = "";
  verts   = {};
}

Mesh::Mesh(string render_type,
	   string meshFile,
	   string solFile,
	   string textureFile){

  ////////////////////////////////////////////////////////////////////////////
  //PREPARING

  //Coherency check
  if(render_type == "PLY_COLORS")
    if((solFile!="") || (textureFile!="") || (meshFile==""))
      cout << "Error while parsing arguments for PLY_COLORS" << endl;
  if(render_type == "PLY_UV")
    if((solFile!="") || (meshFile==""))
      cout << "Error while parsing arguments for PLY_UV" << endl;
  if(render_type == "MESH")
    if((solFile!="") || (textureFile!="") || (meshFile==""))
      cout << "Error while parsing arguments for MESH" << endl;
  if(render_type == "MESH_SOL")
    if((solFile=="") || (textureFile!="") || (meshFile==""))
      cout << "Error while parsing arguments for MESH_SOL" << endl;

  //Separators names
  std::vector<string> NAMES = std::vector<string>();
  if( (render_type == "MESH") || (render_type == "MESH_SOL") ){
    string N[] = {"Vertices", "Triangles", "Normals","NormalAtVertices"};
    NAMES = std::vector<string>(N, N + 4);
  }
  else if(render_type == "PLY_UV"){
    string N[] = {"vertex", "face", "texture_vertex", "texture_face"};
    NAMES = std::vector<string>(N, N + 4);
  }
  else if(render_type == "PLY_COLORS"){
    string N[] = {"vertex", "face"};
    NAMES = std::vector<string>(N, N + 2);
  }
  int numProp = NAMES.size();

  //Preparing the lists
  att attributes[] = {att(),
		      att(),
		      att(),
		      att()};
  //Assigning type variable
  type = render_type;
  //Opening file
  ifstream fin;
  fin.open( meshFile.c_str());
  if ( !fin.good()) 
    cout << "File not opened !!!" << endl;




  ////////////////////////////////////////////////////////////////////////////
  //READING THE FILES

  while (!fin.eof()){
    //Preparation and parsing (common part)
    char buf[MAX_CHARS_PER_LINE];
    fin.getline(buf, MAX_CHARS_PER_LINE);
    int n = 0;
    const char* token[MAX_TOKENS_PER_LINE] = {};
    token[0] = strtok(buf, DELIMITER);
    if (token[0]){
      for (n = 1; n < MAX_TOKENS_PER_LINE; n++){
        token[n] = strtok(0, DELIMITER);
        if (!token[n]) break;
      }
      if(!token[1]){
	token[0] = buf;
      }
    }

    ////////////////////////////////////////////////////
    //Reading PLY files
    if((render_type == "PLY_UV") || (render_type=="PLY_COLORS")){
      //Reading first element
      if(firstFloat){
	if(atof(token[0])){
	  firstFloat=false;
	  readElt[0]=true;
	}
      }
      //Getting sizes
      if( (token[0]) && (!strcmp(token[0],"element")) ){
	for (int i = 0 ; i < numProp ; i++){
	  if(token[1] == NAMES[i]){
	    sizes[i] = atoi(token[2]);
	    attributes[i].size = sizes[i];
	    cout << "sizes["<< NAMES[i] <<"] = " << sizes[i] << endl;
	  }
	}
      }
      //Reading elements
      for(int i = 0 ; i < numProp ; i++){
	if(readElt[i]){
	  std::vector<float> vec;
	  for(int t=0 ; t < n ; t++)
	    vec.push_back(atof(token[t]));
	  attributes[i].val.push_back(vec);
	  eltInc[i]++;
	}
      }
      //Read next element
      for(int i = 0 ; i < numProp ; i++){
	if((readElt[i]) && (eltInc[i] == sizes[i])){
	  readElt[i] = false;
	  if(i<numProp-1)
	    readElt[i+1] = true;
	}
      }
    }

    ////////////////////////////////////////////////////
    //Reading MESH files
    if((render_type == "MESH") || (render_type=="MESH_SOL")){
      //for (int i = 0 ; i < 4 ; i++)
	//cout << readElt[i] << " ";
      //cout << "" << endl;
      //Extract Elements (vertices, triangles...)
      for (int i = 0 ; i < numProp ; i++){
	if(readElt[i]){
	  eltInc[i]++;
	  if (eltInc[i] == sizes[i])
	    readElt[i]=false;
	  //Record the attributes
	  std::vector<float> vec;
	  for(int t=0 ; t < n ; t++)
	    vec.push_back(atof(token[t]));
	  attributes[i].val.push_back(vec);
	  //cout << "0_" << i << endl;
	}
      }
    
      //Extract sizes
      for (int i = 0 ; i < numProp ; i++){
	if(readNum[i]){
	  sizes[i] = atoi(token[0]);
	  attributes[i].size = sizes[i];
	  readNum[i]=false;
	  readElt[i]=true;
	  //cout << "1_" << i << endl;
	}
      }
      //Process line
      for (int tok = 0; tok < n; tok++){
	for (int i = 0 ; i < numProp ; i++){
	  //cout << NAMES[i] << endl;
	  if(token[tok] == NAMES[i]){
	    cout << token[tok] << endl;
	    readNum[i]=true;
	  }
	}
      }
    }
    //Update line number
    lineNumber++;
  }
  cout << "toto" << endl;


  ////////////////////////////////////////////////////////////////////////////
  //POSTPROCESSING and DEBUGGING the values

  //Post Processing vertices
  attributes[0].get_bounding_box();
  attributes[0].get_gravity_center();
  attributes[0].get_scaling_parameters();
  attributes[0].move_and_scale();
  


  ////////////////////////////////////////////////////////////////////////////
  //EXPORTING the values
  
  ///////////////////////////////////////////
  //PLY with UV coordinates, not indexed
  if(render_type == "PLY_UV"){
    //verts
    for(int i = 0 ; i < attributes[1].size ; i++){
      for(int j = 1 ; j < 4 ; j++){
	int indVert = int( attributes[1].val[i][j] -0.0f );
        verts.push_back( attributes[0].val[ indVert ][0] );
	verts.push_back( attributes[0].val[ indVert ][2] );
	verts.push_back( attributes[0].val[ indVert ][1] );
      }
    }
    //uv
    for(int i = 0 ; i < attributes[3].size ; i++){
      for(int j = 1 ; j < 4 ; j++){
	int indUV = int( attributes[3].val[i][j] -0.0f );
	uv.push_back( attributes[2].val[ indUV ][0] );
	uv.push_back( attributes[2].val[ indUV ][1] );
      }
    }
  }

  ///////////////////////////////////////////
  //PLY with integrated colors, indexed
  if(render_type == "PLY_COLORS"){
    //verts and colors
    for(int i = 0 ; i < attributes[0].size ; i++){
      verts.push_back( attributes[0].val[i][0] );
      verts.push_back( attributes[0].val[i][2] );
      verts.push_back( attributes[0].val[i][1] );
      for(int j = 3 ; j < 6 ; j++)
	colors.push_back( attributes[0].val[i][j] / 255.0f );
    }
    //indices
    for(int i = 0 ; i < attributes[1].size ; i++)
      for(int j = 1 ; j < 4 ; j++)
	indices.push_back( int(attributes[1].val[i][j] - 0.0f));
  }

  ///////////////////////////////////////////
  //MESH without .sol file, indexed
  if((render_type == "MESH") || (render_type=="MESH_SOL")){
    //verts
    for(int i = 0 ; i < attributes[0].size ; i++){
      verts.push_back( attributes[0].val[ i ][0] ); //Le x
      verts.push_back( attributes[0].val[ i ][2] ); //Le y
      verts.push_back( attributes[0].val[ i ][1] ); //Le z
    }
    //normals
    for(int i = 0 ; i < 3 * attributes[0].size ; i++)
      normals.push_back(0.0f);
    for(int i = 0 ; i < attributes[3].size ; i++){
      int indVert = int(attributes[3].val[i][0] - 1.0f);
      int indNorm = int(attributes[3].val[i][1] - 1.0f);
      normals[3*indVert + 0] = attributes[2].val[indNorm][0];
      normals[3*indVert + 1] = attributes[2].val[indNorm][2];
      normals[3*indVert + 2] = attributes[2].val[indNorm][1];
    }
    //indices
    for(int i = 0 ; i < attributes[1].size ; i++)
      for(int j = 0 ; j < 3 ; j++)
	indices.push_back( int(attributes[1].val[i][j] - 1.0f));
  }

  ///////////////////////////////////////////
  //MESH with .sol file
  if(render_type=="MESH_SOL"){
    std::ifstream file(solFile.c_str());
    if( !file.good())
      cout << "All bad for sol file !! " << endl;
    std::string str;
    while(std::getline(file, str)){
      colors.push_back(atof(str.c_str()));
    }
  }
}




int att::get_bounding_box(){
  for(int i = 0 ; i < size ; i++){
    for(int j = 0 ; j < 3 ; j++){
      if (i==0){
	mins[j]=val[i][j];
	maxs[j]=val[i][j];
      }
      if(val[i][j] < mins[j])
	mins[j] = val[i][j];
      if(val[i][0] > maxs[j])
	maxs[j] = val[i][j];
    }
  }
  return 1;
}

int att::get_gravity_center(){
  for(int i = 0 ; i < size; i++){
    for(int j = 0 ; j < 3 ; j++){
      gravity_center[j]+=val[i][j];
    }
  }
  for(int j = 0 ; j < 3 ; j++){
    gravity_center[j]/=size;
  }
}

int att::get_scaling_parameters(){
  //Scale parameters
  float s[3];
  for (int j = 0 ; j < 3 ; j++)
    s[j] = maxs[j] - mins[j];
  scaleFactor = 1.0f / max(s[0], max(s[1], s[2])) ;
  //Translation parameters
  for(int j = 0 ; j < 3 ; j++){
    mv[j] = - gravity_center[j];
  }  
  return 1;
}

int att::move_and_scale(){
  for(int i = 0 ; i < size ; i++){
    for(int j = 0 ; j < 3 ; j++){
      val[i][j] += mv[j];
      val[i][j] *= scaleFactor;
    }
  }
  return 1;
}
