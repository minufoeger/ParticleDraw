#pragma once

#include "mesh.h"


class InstancedMesh : public Mesh
{
public:
    InstancedMesh(vector<Vertex> base_verts, vector<unsigned> base_inds,
                  vector<const glm::dvec3 *> inst_attr);
    ~InstancedMesh() override;

    void updateInstAttribs();
    void draw(Shader *shader) override;

private:
    void copyToArrayBuffer();

    GLuint m_inst_VBO;
    vector<const glm::dvec3*> m_inst_attr;
    unsigned m_num_inst;
};