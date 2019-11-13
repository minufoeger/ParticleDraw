#include "framework.h"

#define PI glm::pi<float>()
#define CLEAR_COLOR_BLUE glm::vec4(0.2f, 0.3f, 0.3f, 1.0f)
#define CLEAR_COLOR_BLACK glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
#define CLEAR_COLOR_BLACKISH glm::vec4(0.1f, 0.1f, 0.1f, 1.0f)

//static variables:
static Vertex g_vertices[] = {
        // positions
        Vertex(glm::vec3(0.5f,  0.5f,  0.0f), glm::vec3(1.0f)), // top right
        Vertex(glm::vec3(0.5f, -0.5f,  0.0f), glm::vec3(1.0f)), // bottom right
        Vertex(glm::vec3(-0.5f,-0.5f,  0.0f), glm::vec3(1.0f)), // bottom left
        Vertex(glm::vec3(-0.5f, 0.5f,  0.0f), glm::vec3(1.0f)),  // top left

        Vertex(glm::vec3(0.5f,  0.5f, -1.0f), glm::vec3(1.0f)), // top right
        Vertex(glm::vec3(0.5f, -0.5f, -1.0f), glm::vec3(1.0f)), // bottom right
        Vertex(glm::vec3(-0.5f,-0.5f, -1.0f), glm::vec3(1.0f)), // bottom left
        Vertex(glm::vec3(-0.5f, 0.5f, -1.0f), glm::vec3(1.0f))  // top left

};
static unsigned int g_indices[] = {
        0, 1, 3,    1, 2, 3, //vorne
        4, 5, 7,    5, 6, 7, //hinten
        4, 5, 0,    5, 1, 0, //rechts
        7, 6, 3,    6, 2, 3, //links
        3, 7, 4,    4, 0, 3, //oben
        2, 5, 6,    5, 1, 2  //unten
};

static float pitch = 0.0f, yaw = 0.0f;

Framework::Framework(unsigned width, unsigned height, const string &title,
                     const std::string &vert_path, const std::string &frag_path,
                     const glm::vec3 &cam_pos,
                     vector<glm::dvec3*> *inst_attr)
{
    //initialize the display (window, GLFW, ...)
    m_display = new Display(width, height, title);

    //create view and projection matrix (camera)
    glm::vec3 forward   = -glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f));
    m_camera = new Camera(cam_pos, forward, glm::vec3(0.0f, 1.0f, 0.0f), //pos, forward, up
                          PI/4.0f, ((float)width)/((float)height), //fov, aspect ratio
                          0.3f, 100.0f); //zNear, zFar

    ///////////
    ///BODIES//
    ///////////

    //cube mesh:
    vector<Vertex> vertices;
    for(int i=0; i<sizeof(g_vertices)/sizeof(g_vertices[0]); i++)
        vertices.push_back(g_vertices[i]);
    vector<unsigned> indices;
    for(int i=0; i<sizeof(g_indices)/sizeof(g_indices[0]); i++)
        indices.push_back(g_indices[i]);
    m_cubes = new InstancedMesh(vertices, indices, inst_attr);

    //cube transform:
    m_trans_cube = new Transform(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(0.5f, 0.5f, 0.5f));

    //cube shader:
    m_shader_cube = new Shader(vert_path, frag_path);
    m_shader_cube->setUniform4Matrix4fv("view_mat", m_camera->getView());
    m_shader_cube->setUniform4Matrix4fv("proj_mat", m_camera->getProjection());
    m_shader_cube->setUniform4Matrix4fv("model_mat", m_trans_cube->getModel());


    //init internal thread sync booleans
    m_isUpdate = false;

    //program should be ready to run at this point
    m_running = true;
}


Framework::~Framework()
{
    //Transform
    delete m_trans_cube;
    //Mesh
    delete m_cubes;
    //Shader
    delete m_shader_cube;

    //Camera
    delete m_camera;

    //Display:
    delete m_display;
}


void Framework::start_main_loop()
{
    m_display->inactivateContext();
    m_loop_thread = std::thread(&Framework::thr_main_loop, std::ref(*this));
}

void Framework::join()
{
    m_loop_thread.join();
}

bool Framework::getIsRunning()
{
    std::unique_lock<std::mutex> runLock(m_runMutex);
    return m_running;
}


void Framework::updateInstAttribs()
{
    if(!this->getIsRunning())
        return;

    std::unique_lock<std::mutex> ackLock(m_updateMutex);
    m_isUpdate = true;
    while(m_isUpdate)
        m_ack_cv.wait(ackLock);
}




void Framework::thr_processInput()
{
    //poll wasd keys and update camera position
    float cameraSpeed = 0.05f; // adjust accordingly
    if(m_display->isKeyPressed(GLFW_KEY_W)) {
        m_camera->setPos(m_camera->getPos() + cameraSpeed * m_camera->getForward());
    }else if (m_display->isKeyPressed(GLFW_KEY_S)) {
        m_camera->setPos(m_camera->getPos() - cameraSpeed * m_camera->getForward());
    }else if (m_display->isKeyPressed(GLFW_KEY_A)) {
        glm::vec3 right = glm::normalize(glm::cross(m_camera->getForward(), m_camera->getUp()));
        m_camera->setPos(m_camera->getPos() - cameraSpeed * right);
    }else if (m_display->isKeyPressed(GLFW_KEY_D)) {
        glm::vec3 right = glm::normalize(glm::cross(m_camera->getForward(), m_camera->getUp()));
        m_camera->setPos(m_camera->getPos() + cameraSpeed * right);

        //poll up/down keys to lift/sink vertical
    }else if (m_display->isKeyPressed(GLFW_KEY_UP)) {
        m_camera->setPos(m_camera->getPos() + cameraSpeed * m_camera->getUp());
    }else if (m_display->isKeyPressed(GLFW_KEY_DOWN)) {
        m_camera->setPos(m_camera->getPos() - cameraSpeed * m_camera->getUp());
    }

    //if escape key is pressed, close window
    if(m_display->isKeyPressed(GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        std::unique_lock<std::mutex> runLock(m_runMutex);
        m_running = false;
    }


    //get polled mouse movement and update camera (view matrix):
    //add offsets to corresponding angles
    float sensitivity = 0.1f;
    glm::vec2 offset = m_display->getMouseOffset();
    m_camera->updatePitchYaw(offset.y * sensitivity, offset.x * sensitivity);
}



void Framework::thr_main_loop()
{
    //activate current context on new thread (which main_loop will run on)
    m_display->activateContext();

    //main loop terminates only if m_running is false
    float frametime = 0.0f;
    float lastTime  = glfwGetTime();
    while(this->getIsRunning()) {
        frametime = glfwGetTime() - lastTime;
        //check if window has been closed
        {
            std::unique_lock<std::mutex> runLock(m_runMutex);
            m_running = m_display->windowExistent();
        }

        //check if inst_positions must be updated
        {
            std::unique_lock<std::mutex> ackLock(m_updateMutex);
            if (m_isUpdate) {
                m_cubes->updateInstAttribs();
                m_isUpdate = false;
                m_ack_cv.notify_all();
            }
        }

        //poll display for inputs and process them
        m_display->poll();
        thr_processInput();

        //Clear the screen
        glm::vec4 clearColor = CLEAR_COLOR_BLUE;
        glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_shader_cube->bind();
        m_shader_cube->setUniform4Matrix4fv("view_mat", m_camera->getView());
        m_shader_cube->setUniform4Matrix4fv("model_mat", m_trans_cube->getModel());

        //start pipeline with bound shaders
        m_cubes->draw(m_shader_cube);

        //swap buffers
        m_display->swap();
    }

    //destroy display directly (no waiting for caller to destroy framework instance)
    m_display->destroyWindow();

    //wake up all waiting threads (don't forget to set m_isUpdate!)
    std::unique_lock<std::mutex> ackLock(m_updateMutex);
    m_isUpdate = false;
    m_ack_cv.notify_all();
}




