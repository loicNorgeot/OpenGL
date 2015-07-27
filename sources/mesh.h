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
  float gravity_center[3] = {0,0,0};
  
  float mv[3];         //Move
  float scaleFactor;   //Scale

  int get_bounding_box();
  int get_scaling_parameters();
  int move_and_scale();
  int get_gravity_center();
};

int mesh_read(std::string filename, std::vector<float> &g_vertex, std::vector<float> &g_normal, std::vector<int> &g_indices);


#endif
