#ifndef MESHHEADERDEF
#define MESHHEADERDEF

#include <vector>
#include <string>

class attribute{
public:
  std::vector< std::vector<float> > val;  //Values container
  int size;                               //Number of elements
  float mins[3];
  float maxs[3];
  
  float mv[3];         //Move
  float scaleFactor;   //Scale

  //Pour les vertices
  int get_bounding_box();
  int get_scaling_parameters();
  int move_and_scale();

  //Pour les faces
  int switch_indices();
};

int mesh_read(std::string filename, std::vector<float> &g_vertex);


#endif
