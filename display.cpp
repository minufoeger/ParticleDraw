#include "display.h"

//static function declarations:
static void error_callback(int error, const char* description);
static void framebuffer_size_callback(GLFWwindow *window, int width, int height);
static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

//static variables:
static glm::vec2 g_mouse_pos = glm::vec2();
static glm::vec2 g_last_mouse_pos = glm::vec2();
static glm::vec2 g_mouse_offset = glm::vec2();


Display::Display(unsigned width, unsigned height, const string &title)
{
    //set glfw error callback
    glfwSetErrorCallback(error_callback);

    //init GLFW and create OpenGL window context
    if(initGLFW(width, height, title) != 0) {
        throw runtime_error("failed to create window context!");
    }
}

Display::~Display()
{
}




int Display::initGLFW(unsigned width, unsigned height, const string &title)
{
    glfwSetErrorCallback(error_callback);
    //init glfw
    glewExperimental = GL_TRUE;
    if(!glfwInit()) {
        cerr << "Failed to initialize GLFW" << endl;
        return -1;
    }

    //create context
    glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL

    //create window
    m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if(m_window == nullptr) {
        cerr << "Failed to open GLFW window" << endl;
        glfwTerminate();
        return -1;
    }

    //activate context
    glfwMakeContextCurrent(m_window);
    glewExperimental = GL_TRUE; // Needed in core profile
    if(glewInit() != GLEW_OK) {
        cerr << "Failed to initialize GLEW" << endl;
        return -1;
    }

    //set window resize callback function
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

    //activate depth-buffering
    glEnable(GL_DEPTH_TEST);

    //enable vsync
    glfwSwapInterval(1);

    //enable mouse capturing
    glfwSetCursorPosCallback(m_window, mouse_callback);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    return 0;
}


bool Display::windowExistent()
{
    //check if the ESC key was pressed or the window was closed
    return !glfwWindowShouldClose(m_window);
}

glm::vec2 Display::getMousePos()
{
    return g_mouse_pos;
}

glm::vec2 Display::getMouseOffset()
{
    glm::vec2 temp = g_mouse_offset;
    g_mouse_offset = glm::vec2();
    return temp;
}

bool Display::isKeyPressed(int key)
{
    return glfwGetKey(m_window, key) == GLFW_PRESS;
}

void Display::activateContext()
{
    glfwMakeContextCurrent(m_window);
}

void Display::inactivateContext()
{
    glfwMakeContextCurrent(nullptr);
}

void Display::destroyWindow()
{
    glfwDestroyWindow(m_window); //actually not needed, glfwTerminate destroys all windows
    glfwTerminate();
}


//static function implementations:
static void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    //reset viewport if window resizes
    glViewport(0, 0, width, height);
}

static void error_callback(int error, const char* description)
{
    cerr << "Error " << error << ":" << endl << description << endl;
}


static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    g_mouse_pos = glm::vec2((float)xpos, (float)ypos);

    //smooth out first call
    static int first = true;
    if(first) {
        g_last_mouse_pos = g_mouse_pos;
        first = false;
    }

    //calculate offset
    g_mouse_offset += g_last_mouse_pos - g_mouse_pos;

    //update last_mouse_pos
    g_last_mouse_pos = g_mouse_pos;
}