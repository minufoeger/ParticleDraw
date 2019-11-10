#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using namespace std;

class Display
{
public:
    Display(unsigned width, unsigned height, const string &title);
    ~Display();

    bool windowExistent();
    bool isKeyPressed(int key);

    void swap() { glfwSwapBuffers(m_window); }
    void poll() { glfwPollEvents(); }

    glm::vec2 getMousePos();
    glm::vec2 getMouseOffset();

    void activateContext();
    void inactivateContext();

    void destroyWindow();

private:
    int initGLFW(unsigned width, unsigned height, const string &title);

    GLFWwindow *m_window;
};
