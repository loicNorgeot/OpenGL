#include <vector>
#include <string>

class Mesh
{
 public:
  //Le type de maillage
  std::string type;
  //Les contenants des valeurs
  std::vector<float> verts;
  std::vector<float> colors;
  std::vector<float> normals;
  std::vector<int>   indices;
  std::vector<float> uv;
  //Le constructeur
  Mesh();
  Mesh(std::string render_type,
       std::string meshFile,
       std::string solFile,
       std::string textureFile);
  Mesh& operator=(const Mesh& otherMesh);
};

class att
{
 public:
  std::vector< std::vector<float> > val;
  int size;   
  //Vertices
  float mins[3];
  float maxs[3];
  float gravity_center[3];
  float mv[3];
  float scaleFactor;
  //Méthodes géométriques
  int get_bounding_box();
  int get_scaling_parameters();
  int move_and_scale();
  int get_gravity_center();
};

