#include "storage/Mesh.h"
#include "monitoring/Logsystem.h"

using namespace ogs;

Mesh::Mesh() {
    loginfo("Mesh", "Create simple Mesh");
}

Mesh::~Mesh() {}

void Mesh::init(std::vector<float> vert, std::vector<unsigned int> ind) {
    vertices = vert;
    indices  = ind;
}
