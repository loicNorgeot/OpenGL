#ifndef PLYHEADERDEF
#define PLYHEADERDEF

#include <vector>
#include <string>

class element{
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

int ply_read(std::string plyFile, std::vector<float> &g_ver, std::vector<float> &g_uv);


#endif
