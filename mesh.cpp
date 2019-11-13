#include "mesh.h"

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned> indices)
        : m_vertices(std::move(vertices)), m_indices(std::move(indices))
{
    //init vertexArrayObject and bind it for initialization of vertexBuffers
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    //init all vertexBuffers
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
    //bind vertex data to VBO
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW);
    //bind index data to EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW);


    //bind attribArray 0 to position buffer
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, POS_ELEMS, GL_FLOAT, GL_FALSE,
                          STRIDE_ELEMS * sizeof(float), (void *) 0);

    //bind attribArray 1 to color buffer
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, COL_ELEMS, GL_FLOAT, GL_FALSE,
                          STRIDE_ELEMS * sizeof(float), (void *) ((POS_ELEMS) * sizeof(float)));

    //unbind VAO first and then the buffers
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //draw all Indices
    m_drawCount = m_indices.size();
}


Mesh::~Mesh()
{
    glDeleteBuffers(1, &m_EBO);
    glDeleteBuffers(1, &m_VBO);

    glDeleteVertexArrays(1, &m_VAO);
}

void Mesh::draw(Shader *shader)
{
    //bind VAO and draw it
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_drawCount, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}




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
    glm::vec3 *buf_ptr = (glm::vec3 *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    for(int i=0; i<s_inst_attr->size(); i++) {
        *buf_ptr = *((*s_inst_attr)[i]);
        buf_ptr++;
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);

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

void InstancedMesh::updateInstPos()
{
    //bind vertexArrayObject
    glBindVertexArray(m_VAO);

    //generate VBO and fill with instance data
    glBindBuffer(GL_ARRAY_BUFFER, m_inst_VBO);

    //map buffer, copy everything into it and make sure to unmap after we're done
    glm::vec3 *buf_ptr = (glm::vec3 *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    for(int i=0; i<s_inst_attr->size(); i++) {
        *buf_ptr = *(*s_inst_attr)[i];
        buf_ptr++;
    }
    glUnmapBuffer(GL_ARRAY_BUFFER);


    /*//bind attribArray 2 to instance positions
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
                          (3 + 3) * sizeof(float), (void *) 0);
    glVertexAttribDivisor(2, 1); // tell OpenGL this is an instanced vertex attribute.

    //bind attribArray 3 to instance velocities
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE,
                          (3 + 3) * sizeof(float), (void *) (3 * sizeof(float)));
    glVertexAttribDivisor(3, 1); // tell OpenGL this is an instanced vertex attribute.*/


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

