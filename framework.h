#pragma once

#include <iostream>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <thread>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "display.h"
#include "mesh.h"
#include "shader.h"
#include "camera.h"
#include "transform.h"

using namespace std;

class Framework
{
public:
    Framework(unsigned width, unsigned height, const string &title,
              const std::string &vert_path, const std::string &frag_path,
              const glm::vec3 &cam_pos,
              vector<glm::dvec3*> *inst_attr);
    ~Framework();


    void start_main_loop();
    void join();
    void updateInstPos();
    bool getIsRunning();

private:
    //------ threaded ------//
    void thr_main_loop();

    void thr_processInput();
    //------ threaded ------//


    mutex m_runMutex;
    bool m_running;
    thread m_loop_thread;

    mutex m_updateMutex;
    bool m_isUpdate;
    condition_variable m_ack_cv;


    Display *m_display;
    Camera *m_camera;

    InstancedMesh *m_cubes;
    Shader *m_shader_cube;
    Transform *m_trans_cube;
};
