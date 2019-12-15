#pragma once

#include <iostream>
#include <memory>
#include <mutex>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <GL/glew.h>
#include <vector>

#include "shader.h"

using namespace std;

#define POS_ELEMS 3
#define COL_ELEMS 3
#define STRIDE_ELEMS (POS_ELEMS + COL_ELEMS)

class Vertex
{
public:
    Vertex(glm::vec3 pos, glm::vec3 col)
            : m_pos(pos), m_col(col) {}

    const glm::vec3 &getPos() const { return m_pos; }
    const glm::vec3 &getCol() const { return m_col; }

private:
    glm::vec3 m_pos;
    glm::vec3 m_col;
};



class Mesh
{
public:
    Mesh(vector<Vertex> vertices, vector<unsigned> indices);
    virtual ~Mesh();

    virtual void draw(Shader *shader);

protected:
    GLuint m_VAO;
    GLuint m_VBO, m_EBO;
    unsigned m_drawCount;

    vector<Vertex> m_vertices;
    vector<unsigned> m_indices;
};

