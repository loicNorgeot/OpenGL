#include <string.h>
#include <vector>
#include <glm/glm.hpp>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdio.h>

#include "ply.h"

using namespace std;

const int         MAX_CHARS_PER_LINE  = 512;
const int         MAX_TOKENS_PER_LINE = 20;
const char* const DELIMITER           = " ";

const  string NAMES[]   = {"vertex", "face", "texture_vertex", "texture_face"};
static bool   readElt[] = {false,      false,       false,     false};
static int    eltInc[]  = {0,          0,           0,         0};
static int    sizes[]   = {0,          0,           0,         0};

static int   lineNumber = 0;
static bool  firstFloat = true;


//////////////////////////////////////////////////////////////////////////////////////////////
//Lecture d'un fichier .ply
//La lecture d'un fichier PLY impose de passer toutes les faces au GPU,
//En effet, plusieurs vertices peuvent partager la meme UV texture,
//L'indexation en VBO   NE MARCHE PAS !!!!!!!!!!
//////////////////////////////////////////////////////////////////////////////////////////////

int ply_read( string plyFile,
	      std::vector<float> &g_ver,
	      std::vector<float>   &g_uv){
  //Create the attributes
  element attributes[] = {element(),  //Vertices
			  element(),  //Faces
			  element(),  //Texture vertex
			  element()}; //Texture face
  
  //////////////////////////////////////////////////////
  //MESHFILE READING
  //Opening file
  ifstream fin;
  fin.open( plyFile.c_str());
  if ( !fin.good()) 
    return 0;

  //Read each line
  while (!fin.eof()){
    //Preparing
    char buf[MAX_CHARS_PER_LINE];
    fin.getline(buf, MAX_CHARS_PER_LINE);
    int n = 0;
    const char* token[MAX_TOKENS_PER_LINE] = {};
    
    //Parsing
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

    //Reading first element
    if(firstFloat){
      if(atof(token[0])){
	firstFloat=false;
	readElt[0]=true;
      }
    }

    //Getting sizes
    if( (token[0]) && (!strcmp(token[0],"element")) ){
      for (int i = 0 ; i < 4 ; i++){
	if(token[1] == NAMES[i]){
	  sizes[i] = atoi(token[2]);
	  attributes[i].size = sizes[i];
	  cout << "sizes["<< NAMES[i] <<"] = " << sizes[i] << endl;
	}
      }
    }

    //Reading elements
    for(int i = 0 ; i < 4 ; i++){
      if(readElt[i]){
	std::vector<float> vec;
	for(int t=0 ; t < n ; t++)
	  vec.push_back(atof(token[t]));
	attributes[i].val.push_back(vec);
	eltInc[i]++;
      }
    }

    //Read next element
    for(int i = 0 ; i < 4 ; i++){
      if((readElt[i]) && (eltInc[i] == sizes[i])){
	readElt[i] = false;
	if(i<3)
	  readElt[i+1] = true;
      }
    }

    //Update line number
    lineNumber++;
  }

  //POSTPROCESSING VERTICES
  attributes[0].get_bounding_box();
  attributes[0].get_gravity_center();
  attributes[0].get_scaling_parameters();
  attributes[0].move_and_scale();
  
  //Debugging
  cout << "\n//////////    DEBUGGING    //////////" << endl;
  cout << attributes[0].val[0][0] << " "
       << attributes[0].val[0][1] << " "
       << attributes[0].val[0][2] << endl;
  cout << attributes[0].val[sizes[0] - 1][0] << " "
       << attributes[0].val[sizes[0] - 1][1] << " "
       << attributes[0].val[sizes[0] - 1][2] << endl;
  cout << attributes[0].mins[0] << " " << attributes[0].mins[1] << endl;
  cout << attributes[0].scaleFactor << endl;
  cout << attributes[0].gravity_center[0] << " "
       << attributes[0].gravity_center[1] << " "
       << attributes[0].gravity_center[2] << endl;
  cout << " " << endl;
  
  //Remplissage de g_vertex
  for(int i = 0 ; i < attributes[1].size ; i++){
    for(int j = 1 ; j < 4 ; j++){
      int indVert = int( attributes[1].val[i][j] -0.0f );
      if((indVert<0) || (indVert>attributes[0].size)){
	cout << i << " :\t" << indVert << " / " << attributes[0].size << endl;
   	cout << "ça va pas pas pas" << endl;
      }
      g_ver.push_back( attributes[0].val[ indVert ][0] );
      g_ver.push_back( attributes[0].val[ indVert ][2] );
      g_ver.push_back( attributes[0].val[ indVert ][1] );
    }
  }

  //Remplissage de g_uv
  for(int i = 0 ; i < attributes[3].size ; i++){
    for(int j = 1 ; j < 4 ; j++){
      int indUV = int( attributes[3].val[i][j] -0.0f );
      g_uv.push_back( attributes[2].val[ indUV ][0] );
      g_uv.push_back( attributes[2].val[ indUV ][1] );
    }
  }

  cout << "Nb vertices = \t" << g_ver.size() << endl;
  cout << "Nb uv_coord = \t" << g_uv.size() << endl;
  //End Of Function
  return 1;
}

//int get_bounding_box(int &mx, int &Mx, int &my, int &My, int &mz, int &Mz){
int element::get_bounding_box(){
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

int element::get_gravity_center(){
  for(int i = 0 ; i < size; i++){
    for(int j = 0 ; j < 3 ; j++){
      gravity_center[j]+=val[i][j];
    }
  }
  for(int j = 0 ; j < 3 ; j++){
    gravity_center[j]/=size;
  }
}

int element::get_scaling_parameters(){
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

int element::move_and_scale(){
  for(int i = 0 ; i < size ; i++){
    for(int j = 0 ; j < 3 ; j++){
      val[i][j] += mv[j];
      val[i][j] *= scaleFactor;
    }
  }
  return 1;
}


