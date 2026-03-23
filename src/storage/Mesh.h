#ifndef OGS_MESH_H
#define OGS_MESH_H

#include <vector>
#include "storage/Resource.h"

namespace ogs {

class Mesh : public Resource {
private:
    const int mask = 0x02; // T_MESH
    std::vector<unsigned int> indices;
    std::vector<float> vertices;
    std::vector<float> colors;
    std::vector<float> uv;

public:
    Mesh();
    ~Mesh();

    void setIndices(std::vector<unsigned int> ind)  { indices = ind; }
    void setVertices(std::vector<float> vert)       { vertices = vert; }

    void init(std::vector<float> vert, std::vector<unsigned int> ind);

    std::vector<float>& getVertices()               { return vertices; }
    std::vector<unsigned int>& getIndices()         { return indices; }

    int getMask() override                          { return mask; }
};

} // namespace ogs

#endif
