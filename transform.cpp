#include "transform.h"

Transform::Transform()
        : m_pos(0.0f, 0.0f, 0.0f), m_quat_rot(glm::quat()), m_scale(1.0f, 1.0f, 1.0f)
{
}

Transform::Transform(const glm::vec3 &pos, const glm::vec3 &rot, const glm::vec3 &scale)
        : m_pos(pos), m_quat_rot(glm::quat(rot)), m_scale(scale)
{
}

Transform::Transform(const glm::vec3 &pos, const glm::vec3 &rotAxis, float rotAngle, const glm::vec3 &scale)
        : m_pos(pos), m_quat_rot(glm::angleAxis(rotAngle, rotAxis)), m_scale(scale)
{
}


glm::mat4 Transform::getModel() const
{
    glm::mat4 transMat  = glm::translate(m_pos);
    glm::mat4 rotMat    = glm::toMat4(m_quat_rot);
    glm::mat4 scaleMat  = glm::scale(m_scale);

    return transMat * rotMat * scaleMat;
}

