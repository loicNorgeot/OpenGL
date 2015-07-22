#ifndef MESHHEADERDEF
#define MESHHEADERDEF

#include <vector>
#include <string>

class attribute{
public:
  std::vector< std::vector<float> > val;
};

int mesh_read(std::string filename = "/home/loic/Blender/MESH/FauduetMuscle.o1.mesh");

#endif
