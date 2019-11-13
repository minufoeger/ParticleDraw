#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <glm/glm.hpp>

#include "framework.h"

#define ARRAY_SIZE 3

using namespace std;

glm::vec3 g_array[ARRAY_SIZE*ARRAY_SIZE*2] = { glm::vec3(0.0f) };
mutex g_posMutex;

int main()
{
    vector<glm::vec3 *> g_inst_positions;

    //create array and construct vector from it
    for(int i=0; i<ARRAY_SIZE; ++i) {
        for(int j=0; j<ARRAY_SIZE; ++j) {
            float x = (float)(-ARRAY_SIZE/2+j);
            float y = (float)(-ARRAY_SIZE/2+i);
            g_array[2*(i*ARRAY_SIZE+j)] = glm::vec3(x, y, 0.0f);
            g_array[2*(i*ARRAY_SIZE+j)+1] = glm::vec3(1.0f, 1.0f, 0.0f);
        }
    }
    for(int i=0; i<sizeof(g_array)/sizeof(g_array[0]); i++)
        g_inst_positions.push_back(&g_array[i]);


    //init a Framework and start it's main loop (new thread)
    Framework fr(800, 600, "ParticleDraw",
                 "../res/instanced_shader.vert", "../res/instanced_shader.frag",
                 glm::vec3(0.0f, 0.0f, 5.0f),
                 &g_inst_positions);
    fr.start_main_loop();

    //do for ~10s
    for(int i=0; i<100; ++i) {
        cout << "Hello start: " << i << endl;
        //update positions
        g_posMutex.lock();
        for (auto it = g_inst_positions.begin(); it != g_inst_positions.end(); it+=2) {
            **it += glm::vec3(0.8f, 0.0f, 0.0f);
        }
        g_posMutex.unlock();

        //notify thread
        if(!fr.getIsRunning())
            break;
        fr.updateInstPos();

        //sleep some time
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        cout << "Hello middle: " << i << endl;

        //update positions
        g_posMutex.lock();
        for (auto it = g_inst_positions.begin(); it != g_inst_positions.end(); it+=2) {
            **it -= glm::vec3(0.8f, 0.0f, 0.0f);
        }
        g_posMutex.unlock();

        //notify thread
        if(!fr.getIsRunning())
            break;
        fr.updateInstPos();

        //sleep some time
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        cout << "Hello end: " << i << endl;
    }

    cout << "joining..." << endl;
    fr.join();
    cout << "joined!" << endl;

    return 0;
}