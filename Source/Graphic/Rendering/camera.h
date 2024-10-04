#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera {
public:
    float m_movementSpeed;
    float m_mouseSensitivity;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 5.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = -90.0f,
        float pitch = 0.0f);

	void processKeyboard(CameraMovement direction, double deltaTime);
	void processMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
	//void processMouseScroll();

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();
    glm::vec3 getPosition();
private:
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;
    glm::vec3 m_worldUp;

    float m_yaw;
    float m_pitch;

    //float m_movementSpeed;
    //float m_mouseSensitivity;
    float m_zoom;

    void updateCameraVectors();
};

#endif 
