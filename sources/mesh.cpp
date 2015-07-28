#include <string.h>
#include <vector>
#include <glm/glm.hpp>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdio.h>

#include "mesh.h"

using namespace std;

const int MAX_CHARS_PER_LINE = 512;
const int MAX_TOKENS_PER_LINE = 20;
const char* const DELIMITER = " ";

const  string NAMES[]   = {"Vertices", "Triangles", "Normals", "NormalAtVertices"};
static bool   readNum[] = {false,      false,       false,     false};
static bool   readElt[] = {false,      false,       false,     false};
static int    eltInc[]  = {0,          0,           0,         0};
static int    sizes[]   = {0,          0,           0,         0};

static int lineNumber = 0;


//Lecture d'un fichier .mesh
int mesh_read( string meshFile, 
	       string solFile,
	       std::vector<float> &g_vertex,
	       std::vector<float> &g_normal,
	       std::vector<int>   &g_indices,
	       std::vector<float> &g_solution){

  //Create the attributes
  attribute attributes[] = {attribute(),//Vertices
			    attribute(),//Triangles
			    attribute(),//Normals
			    attribute()};//NormalAtVertices
  
  //////////////////////////////////////////////////////
  //MESHFILE READING
  ifstream fin;
  fin.open( meshFile.c_str());
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
    }

    //Extract Elements (vertices, triangles...)
    for (int i = 0 ; i < 4 ; i++){
      if(readElt[i]){
	eltInc[i]++;
	if (eltInc[i] == sizes[i])
	  readElt[i]=false;
	//Record the attributes
	std::vector<float> vec;
	for(int t=0 ; t < n ; t++)
	  vec.push_back(atof(token[t]));
	attributes[i].val.push_back(vec);
      }
    }
    
    //Extract sizes
    for (int i = 0 ; i < 4 ; i++){
      if(readNum[i]){
	sizes[i] = atoi(token[0]);
	attributes[i].size = sizes[i];
	readNum[i]=false;
	readElt[i]=true;
      }
    }

    //Process line
    for (int tok = 0; tok < n; tok++){
      for (int i = 0 ; i < 4 ; i++){
	if(token[tok]==NAMES[i]){
	  readNum[i]=true;
	}
      }
    }

    //Update line number
    lineNumber++;
  }


  //////////////////////////////////////////////////////
  //SOLFILE READING
  std::ifstream file(solFile);
  if( !file.good())
    return 0;
  std::string str;
  while(std::getline(file, str)){
    g_solution.push_back(stof(str));
  }
    
   
  //POSTPROCESSING VERTICES
  attributes[0].get_bounding_box();
  attributes[0].get_gravity_center();
  attributes[0].get_scaling_parameters();
  attributes[0].move_and_scale();

  ////////////////////////////////////////////////////////////////////////////////////////
  //REMPLISSAGE AVEC INDEXATION

  //Remplissage de g_vertex
  for(int i = 0 ; i < attributes[0].size ; i++){
      g_vertex.push_back( attributes[0].val[ i ][0] ); //Le x
      g_vertex.push_back( attributes[0].val[ i ][2] ); //Le y
      g_vertex.push_back( attributes[0].val[ i ][1] ); //Le z
  }

  //Remplissage de g_normal
  for(int i = 0 ; i < 3 * attributes[0].size ; i++)
    g_normal.push_back(0.0f);//Gère le cas ou il n'y a pas de normale correspondante
  for(int i = 0 ; i < attributes[3].size ; i++){
    int indVert = int(attributes[3].val[i][0] - 1.0f);
    int indNorm = int(attributes[3].val[i][1] - 1.0f);
    g_normal[3*indVert + 0] = attributes[2].val[indNorm][0];
    g_normal[3*indVert + 1] = attributes[2].val[indNorm][2];
    g_normal[3*indVert + 2] = attributes[2].val[indNorm][1];
  }

  //Remplissage de g_indices
  for(int i = 0 ; i < attributes[1].size ; i++)
    for(int j = 0 ; j < 3 ; j++)
      g_indices.push_back( int(attributes[1].val[i][j] - 1.0f));
  
  //End Of Function
  return 1;
}

//int get_bounding_box(int &mx, int &Mx, int &my, int &My, int &mz, int &Mz){
int attribute::get_bounding_box(){
  
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

int attribute::get_gravity_center(){
  for(int i = 0 ; i < size; i++){
    for(int j = 0 ; j < 3 ; j++){
      gravity_center[j]+=val[i][j];
    }
  }
  for(int j = 0 ; j < 3 ; j++){
    gravity_center[j]/=size;
    cout << "grav " << j << " = " << gravity_center[j] << endl;
  }
}

int attribute::get_scaling_parameters(){
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

int attribute::move_and_scale(){
  for(int i = 0 ; i < size ; i++){
    for(int j = 0 ; j < 3 ; j++){
      val[i][j] += mv[j];
      val[i][j] *= scaleFactor;
    }
  }
  return 1;
}


