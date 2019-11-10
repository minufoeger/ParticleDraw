#include "camera.h"

Camera::Camera(const glm::vec3 &pos, const glm::vec3 &forward, const glm::vec3 &up,
               float fov, float aspect, float zNear, float zFar)
        : m_pos(pos), m_forward(forward), m_up(up)
{
    //get projection matrix
    m_proj = glm::perspective(fov, aspect, zNear, zFar);

    //also initialize pitch and yaw
    //-> calculate them from the given forward vector (with 0.0f forward is (0,0,1))
    m_pitch   = glm::degrees(asin(forward.y));
    m_yaw     = glm::degrees(atan2(forward.x, forward.z));
}

glm::mat4 Camera::getProjectedView()
{
    return m_proj * glm::lookAt(m_pos, m_pos + m_forward, m_up);
}

glm::vec3 *Camera::getOrthoSystem()
{
    auto *coords = new glm::vec3[3];
    coords[0] = glm::normalize(glm::cross(m_up, m_forward));
    coords[1] = glm::normalize(glm::cross(coords[0], m_forward));
    coords[2] = glm::normalize(m_forward);

    return coords;
}

void Camera::updatePitchYaw(float p_offset, float y_offset)
{
    m_pitch += p_offset;
    m_yaw   += y_offset;

    //pitch shouldn't be allowed to grow infinitely
    if(m_pitch > 89.0f) m_pitch =  89.0f;
    if(m_pitch < -89.0f) m_pitch = -89.0f;

    //calculate mouseFront variable to which the camera forward vector is set to
    //with pitch/yaw = 0.0f forward is (0,0,1)
    glm::vec3 forward = glm::vec3(
            cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw)),
            sin(glm::radians(m_pitch)),
            cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw))
    );
    m_forward = glm::normalize(forward);
}

