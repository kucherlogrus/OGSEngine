#ifndef OGS_MESH_H
#define OGS_MESH_H

#include "backends/GLConfig.h"
#include "Macroses.h"
#include "storage/Resource.h"

namespace ogs {

    class Mesh : public Resource {

    private:
        std::vector<unsigned int> indices;
        std::vector<float> verticles;
        std::vector<float> colors;
        std::vector<float> uv;

        const int mask = T_MESH ;

    public:

        Mesh();

        ~Mesh();

        void setIndices(std::vector<unsigned int> ind) {
            indices = ind;
        }

        void setVerticles(std::vector<float> vert) {
            verticles = vert;
        }


        void init(const std::vector<float> vert, std::vector<unsigned int> ind);

        std::vector<float>& getVerticles(){
            return verticles;
        }

        std::vector<unsigned int>& getIndices(){
            return indices;
        }

        void invalidate();

        virtual const int getMask() { return mask; }

    };
}


#endif
