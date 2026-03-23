#include "Engine.h"
#include "Mesh.h"


using namespace ogs;

Mesh::Mesh() {
    loginfo("Mesh", "Create simple Mesh");
};

Mesh::~Mesh() {

};


void Mesh::init(const std::vector<float> vert, std::vector<unsigned int> ind ) {
    verticles = vert;
    indices = ind;
}


void Mesh::invalidate(){
    //TODO - dynamic load data
    //programID = CORE->getRenderer()->getShaderManager()->createShaderDefault();
    /*
    glUseProgram(programID);
    auto a_pos  = glGetAttribLocation(programID, "a_position");
    auto a_col  = glGetAttribLocation(programID, "a_color");
    auto a_uv  = glGetAttribLocation(programID, "a_uv");

    glGenBuffers(1, &vertexbufferID);
    glGenBuffers(1, &colorbufferID);
    glGenBuffers(1, &indexbufferID);
    glGenBuffers(1, &uvbufferID);

    glBindBuffer(GL_ARRAY_BUFFER, vertexbufferID);
    glBufferData(GL_ARRAY_BUFFER, verticles.size() * sizeof(float), &verticles[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, colorbufferID);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), &colors[0], GL_DYNAMIC_DRAW);


    glBindBuffer(GL_ARRAY_BUFFER, uvbufferID);
    glBufferData(GL_ARRAY_BUFFER, uv.size() * sizeof(float) , &uv[0] , GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(a_pos);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbufferID);
    glVertexAttribPointer(a_pos, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(a_col);
    glBindBuffer(GL_ARRAY_BUFFER, colorbufferID);
    glVertexAttribPointer(a_col, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(a_uv);
    glBindBuffer(GL_ARRAY_BUFFER, uvbufferID);
    glVertexAttribPointer(a_uv, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
     */

}




