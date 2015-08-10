#ifndef LOADERHEADERDEF
#define LOADERHEADERDEF

#include <vector>
#include <string>

/**
 * \brief       Class for a .ply or .mesh mesh
 *
 * \details     Contains the informations needed for a mesh rendering.
 */
class MESH
{
 public:
  std::string type;
  std::vector<float> verts;
  std::vector<float> colors;
  std::vector<float> normals;
  std::vector<int>   indices;
  std::vector<float> uv;

  /**
   * \brief       Default constructor
   */
  MESH();

  /**
   * \brief       Reads .mesh or .ply files, and stores the data
   *
   * \details     Depending on the provided types and files,
   * reads the relevant mesh attributes from files, and stores in a convenient way.
   * \param   render_type  either PLY_UV, PLY_COLORS, MESH_SOL or MESH
   * \param   meshFile     path to .mesh file
   * \param   solFile      path to a .sol file
   * \param   textureFile  path to a texture image
   * \return  A mesh instance
   */
  MESH(std::string render_type,
       std::string meshFile,
       std::string solFile,
       std::string textureFile);

  /**
   * \brief       Overloads the assignement operator
   */
  MESH& operator=(const MESH& otherMesh);
};





/**
 * \brief       Class used for parsing a mesh file
 *
 * \details     Contains a vector container of the different attributes,
 * as well as methods modifying the vertices information.
 */
class att
{
 public:
  std::vector< std::vector<float> > val;
  int size;
  float mins[3];
  float maxs[3];
  float gravity_center[3];
  float mv[3];
  float scaleFactor;

  /**
   * \brief       Computes a bounding box from a vector of vertices
   *
   * \return  1 on success
   */
  int get_bounding_box();

  /**
   * \brief       Computes the necessary scaling factor
   *
   * \return  1 on success
   */
  int get_scaling_parameters();

  /**
   * \brief       Translates and scales the vertices positions
   *
   * \return  1 on success
   */
  int move_and_scale();

  /**
   * \brief       Computes the mesh gravity center in 3D
   *
   * \return  1 on success
   */
  int get_gravity_center();
};

#endif
