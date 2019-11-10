#include "shader.h"

Shader::Shader(const string &vertexFilePath, const string &fragmentFilePath) : m_max_lights(glm::uvec3(0,0,0))
{
    // Create the shaders
    m_vs = glCreateShader(GL_VERTEX_SHADER);
    m_fs = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file and compile it
    compile_shader(m_vs, load_shader(vertexFilePath));
    compile_shader(m_fs, load_shader(fragmentFilePath));

    //create shader program and link shaders to it
    m_program = glCreateProgram();
    glAttachShader(m_program, m_vs);
    glAttachShader(m_program, m_fs);
    glLinkProgram(m_program);
    check_shader_error(m_program, true, GL_LINK_STATUS);

    //detach vs/fs ids from program (not needed any more for it's been linked together already)
    //and delete them from graphics memory (removal of source code/object file boilerplate)
    glDetachShader(m_program, m_vs);
    glDeleteShader(m_vs);
    glDetachShader(m_program, m_fs);
    glDeleteShader(m_fs);
}

Shader::~Shader()
{
    glDeleteProgram(m_program);
}



void Shader::bind()
{
    glUseProgram(m_program);
}


GLint Shader::getUniformLocation(const string &name)
{
    return glGetUniformLocation(m_program, name.c_str());
}



string Shader::load_shader(const string &fileName)
{
    ifstream file;
    stringstream ss;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        file.open(fileName);
        ss << file.rdbuf();

    } catch(ifstream::failure &e) {
        cerr << "Failed to load shader:" << endl << e.what() << ", " << e.code() << endl;
    }
    return ss.str();
}

void Shader::compile_shader(GLuint shader, const string &code)
{
    //compile shader
    GLchar const *sourcePointer = code.c_str();
    glShaderSource(shader, 1, &sourcePointer, nullptr);
    glCompileShader(shader);

    //check if shader compiled right
    check_shader_error(shader, false, GL_COMPILE_STATUS);
}

void Shader::check_shader_error(GLuint id, bool isProgram, GLuint flag)
{
    //check shader
    GLint result = GL_FALSE, infoLogLen = 0;

    if(isProgram) {
        glGetProgramiv(id, flag, &result);
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLogLen);
    } else {
        glGetShaderiv(id, flag, &result);
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLen);
    }

    if(infoLogLen > 0) {
        //System error message
        vector<char> error_msg ((unsigned)infoLogLen + 1);
        if(isProgram) {
            glGetProgramInfoLog(id, infoLogLen, nullptr, &error_msg[0]);
            cerr << "Program linking error:" << endl;
        } else {
            glGetShaderInfoLog(id, infoLogLen, nullptr, &error_msg[0]);
            cerr << "Shader compilation error:" << endl;
        }
        cerr << &error_msg[0] << endl;
    }
}

