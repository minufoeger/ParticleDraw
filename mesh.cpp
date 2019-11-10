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

    //debind vertexArrayObject
    glBindVertexArray(0);

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




InstancedMesh::InstancedMesh(vector<Vertex> base_verts, vector<unsigned> base_inds, vector<glm::vec3> *inst_positions)
    : Mesh::Mesh(std::move(base_verts), std::move(base_inds)), s_inst_positions(inst_positions)
{
    m_num_pos = s_inst_positions->size();

    //bind vertexArrayObject
    glBindVertexArray(m_VAO);

    //generate VBO and fill with instance data
    glGenBuffers(1, &m_inst_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_inst_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * s_inst_positions->size(),
                 s_inst_positions->data(), GL_STATIC_DRAW);

    //bind attribArray 2 to instance positions
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
                          3 * sizeof(float), (void *) 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(2, 1); // tell OpenGL this is an instanced vertex attribute.

    //unbind VAO and delete buffer
    glBindVertexArray(0);
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * s_inst_positions->size(),
                 s_inst_positions->data(), GL_STATIC_DRAW);

    //bind attribArray 2 to instance positions
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
                          3 * sizeof(float), (void *) 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(2, 1); // tell OpenGL this is an instanced vertex attribute.

    //unbind VAO and delete buffer
    glBindVertexArray(0);
}

void InstancedMesh::draw(Shader *shader)
{
    //bind VAO and draw it
    glBindVertexArray(m_VAO);
    glDrawElementsInstanced(GL_TRIANGLES, m_drawCount, GL_UNSIGNED_INT, nullptr, m_num_pos);
    glBindVertexArray(0);
}

