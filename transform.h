#pragma once

#include <iostream>
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

using namespace std;

class Transform
{
public:
    Transform();
    Transform(const glm::vec3 &pos, const glm::vec3 &rot, const glm::vec3 &scale);
    Transform(const glm::vec3 &pos, const glm::vec3 &rotAxis, float rotAngle, const glm::vec3 &scale);

    glm::mat4 getModel() const;

    inline void setPos(glm::vec3 pos) { m_pos = pos; }

    inline void setRotQuat(glm::quat rot) { m_quat_rot = rot; }
    inline void setRotEul(glm::vec3 rot) { m_quat_rot = glm::quat(rot); }
    inline void setRotAxis(const glm::vec3 &rotAxis, float rotAngle) { m_quat_rot = glm::angleAxis(rotAngle, rotAxis); }

    inline void setScale(glm::vec3 scale) { m_scale = scale; }

    inline glm::vec3 getPos() { return m_pos; }
    inline glm::quat getRot() { return m_quat_rot; }
    inline glm::vec3 getScale() { return m_scale; }

private:
    glm::vec3 m_pos, m_scale;
    glm::quat m_quat_rot;
};
