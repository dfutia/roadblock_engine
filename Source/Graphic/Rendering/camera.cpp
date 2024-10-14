#include "Graphic/Rendering/camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) :
    m_position(position), m_worldUp(up), m_yaw(yaw), m_pitch(pitch),
    m_front(glm::vec3(0.0f, 0.0f, -1.0f)), m_movementSpeed(2.5f),
    m_mouseSensitivity(0.1f), m_zoom(70.0f)
{
    updateCameraVectors();
}

void Camera::processKeyboard(CameraMovement direction, double deltaTime) {
    float velocity = m_movementSpeed * deltaTime;
    if (direction == FORWARD)
        m_position += m_front * velocity;
    if (direction == BACKWARD)
        m_position -= m_front * velocity;
    if (direction == LEFT)
        m_position -= m_right * velocity;
    if (direction == RIGHT)
        m_position += m_right * velocity;
}

void Camera::processMouseMovement(float xOffset, float yOffset, bool constrainPitch) {
    xOffset *= m_mouseSensitivity;
    yOffset *= m_mouseSensitivity;

    m_yaw += xOffset;
    m_pitch -= yOffset;

    if (constrainPitch)
    {
        if (m_pitch > 89.0f)
            m_pitch = 89.0f;
        if (m_pitch < -89.0f)
            m_pitch = -89.0f;
    }

    updateCameraVectors();
}

//void Camera::processMouseScroll()
//{
//}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

glm::mat4 Camera::getProjectionMatrix() {
    return glm::perspective(glm::radians(m_zoom), m_aspectRatio, 0.1f, 1000.0f);
}

glm::vec3 Camera::getPosition() {
    return m_position;
}

void Camera::updateCameraVectors() {
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    newFront.y = sin(glm::radians(m_pitch));
    newFront.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(newFront);
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}
