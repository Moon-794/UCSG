#ifndef U_VOXEL
#define U_VOXEL

struct Voxel
{    
    Voxel(int x, int y, int z);

    //Number of bits along each dimension
    const int size = 16;
};

class Mesh
{
    Mesh();

    void SetVertices(const std::vector<Vertex> vertices);
    void SetIndices(const std::vector<unsigned int> indices);

private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};

//Loop through each voxel and slap a face on each exposed side.
Mesh BuildVoxelMesh(const Voxel& voxel);



#endif