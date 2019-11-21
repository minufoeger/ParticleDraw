#include "inst_mesh.h"

InstancedMesh::InstancedMesh(vector<Vertex> base_verts, vector<unsigned> base_inds, vector<glm::dvec3*> *inst_attr)
        : Mesh::Mesh(std::move(base_verts), std::move(base_inds)), s_inst_attr(inst_attr)
{
    m_num_inst = s_inst_attr->size()/2;

    //bind vertexArrayObject
    glBindVertexArray(m_VAO);

    //generate and bind VBO for instanced data
    glGenBuffers(1, &m_inst_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_inst_VBO);
    //reserve space on GPU memory
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * s_inst_attr->size(), nullptr, GL_STATIC_DRAW);
    //map buffer, copy everything into it and make sure to unmap after we're done
    copyToArrayBuffer();

    //bind attribArray 2 to instance positions
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
                          (3 + 3) * sizeof(float), (void *) 0);
    glVertexAttribDivisor(2, 1); // tell OpenGL this is an instanced vertex attribute.

    //bind attribArray 3 to instance velocities
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE,
                          (3 + 3) * sizeof(float), (void *) (3 * sizeof(float)));
    glVertexAttribDivisor(3, 1); // tell OpenGL this is an instanced vertex attribute.


    //unbind VAO first and then the buffers
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

InstancedMesh::~InstancedMesh()
{
    glDeleteBuffers(1, &m_inst_VBO);
}

void InstancedMesh::updateInstAttribs()
{
    //bind vertexArrayObject
    glBindVertexArray(m_VAO);

    //generate VBO and fill with instance data
    glBindBuffer(GL_ARRAY_BUFFER, m_inst_VBO);

    //map buffer, copy everything into it and make sure to unmap after we're done
    copyToArrayBuffer();

    //unbind VAO first and then the buffers
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void InstancedMesh::draw(Shader *shader)
{
    //bind VAO and draw it
    glBindVertexArray(m_VAO);
    glDrawElementsInstanced(GL_TRIANGLES, m_drawCount, GL_UNSIGNED_INT, nullptr, m_num_inst);
    glBindVertexArray(0);
}



void InstancedMesh::copyToArrayBuffer()
{
    //map buffer, copy everything into it and make sure to unmap after we're done
    auto *buf_ptr = (glm::vec3 *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    for(int i=0; i<(int)(s_inst_attr->size()/2); i++) {
        *buf_ptr = *((*s_inst_attr)[2*i]);
        buf_ptr++;
        double norm = glm::l2Norm(*((*s_inst_attr)[2*i+1]))/10.0f;
        *buf_ptr = glm::vec3((float)norm, 0.0f, 0.0f);
        buf_ptr++;
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);
}