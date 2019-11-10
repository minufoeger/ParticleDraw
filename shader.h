#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

using namespace std;

class Shader
{
public:
    Shader(const string &vertexFilePath, const string &fragmentFilePath);
    ~Shader();

    void bind();

    glm::uvec3 getMaxLights() { return m_max_lights; }
    void setMaxLights(const glm::uvec3 &maxLights) { m_max_lights = maxLights; }

    GLint getUniformLocation(const string &name);

    void setUniform1i(const string &name, GLint val)
    { this->bind(); glUniform1i(getUniformLocation(name), val); }
    void setUniform1f(const string &name, GLfloat val)
    { this->bind(); glUniform1f(getUniformLocation(name), val); }
    void setUniform3fv(const string &name, const glm::vec3 &val)
    { this->bind(); glUniform3fv(getUniformLocation(name), 1, &val[0]); }
    void setUniform4Matrix4fv(const string &name, const glm::mat4 &val)
    { this->bind(); glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &val[0][0]); }

private:
    string load_shader(const string &fileName);
    void compile_shader(GLuint shader, const string &code);
    void check_shader_error(GLuint id, bool isProgram, GLuint flag);

    GLuint m_program, m_vs, m_fs;
    glm::uvec3 m_max_lights;
};
