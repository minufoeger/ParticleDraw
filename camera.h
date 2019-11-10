#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Camera
{
public:
    Camera(const glm::vec3 &pos, const glm::vec3 &forward, const glm::vec3 &up,
           float fov, float aspect, float zNear, float zFar);


    glm::mat4 getProjection() { return m_proj; }
    glm::mat4 getView() { return glm::lookAt(m_pos, m_pos + m_forward, m_up); }
    glm::mat4 getProjectedView();

    glm::vec3 *getOrthoSystem();
    void updatePitchYaw(float p_offset, float y_offset);

    const glm::vec3 &getPos() const { return m_pos; }
    void setPos(const glm::vec3 &pos) { m_pos = pos; }

    const glm::vec3 &getForward() const { return m_forward; }
    void setForward(const glm::vec3 &forward) { m_forward = forward; }

    const glm::vec3 &getUp() const { return  m_up; }
    void setUp(const glm::vec3 &up) { m_up = up; }

private:
    glm::vec3 m_pos, m_forward, m_up;
    glm::mat4 m_proj;
    float m_pitch, m_yaw;
};
