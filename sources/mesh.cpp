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
int mesh_read( string filename){
  
  //Create the attributes
  attribute attributes[] = {attribute(),//Vertices
			    attribute(),//Triangles
			    attribute(),//Normals
			    attribute()};//NormalAtVertices
  
  //Open a file
  ifstream fin;
  fin.open( filename.c_str());
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
	readNum[i]=false;
	readElt[i]=true;
      }
    }

    //Process line
    for (int tok = 0; tok < n; tok++){
      for (int i = 0 ; i < 4 ; i++){
	if(token[tok]==NAMES[i]){
	  //cout << buf << endl;
	  readNum[i]=true;
	}
      }
    }

    //Update line number
    lineNumber++;

  }
  cout << attributes[0].val[20797][0] << endl;
}
