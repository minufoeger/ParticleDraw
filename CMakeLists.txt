cmake_minimum_required(VERSION 3.15)
project(ParticleDraw)
set(CMAKE_CXX_STANDARD 14)

add_library(ParticleDraw STATIC framework.cpp framework.h display.cpp display.h camera.cpp camera.h shader.cpp shader.h mesh.cpp mesh.h transform.cpp transform.h)

# glew
find_package(GLEW REQUIRED)
# opengl
find_package(OpenGL REQUIRED)
# glfw
find_package(glfw3 REQUIRED)
# posix threads
find_package(Threads)

target_include_directories(ParticleDraw
        PUBLIC
        ${GLEW_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIRS}
)

# target libraries
target_link_libraries(ParticleDraw
        glfw
        ${OPENGL_LIBRARIES}
        ${GLEW_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        ${GLFW_LIBRARY_DIRS}
)


# main executable
add_executable(test_program main.cpp)
target_link_libraries(test_program
        ParticleDraw
)