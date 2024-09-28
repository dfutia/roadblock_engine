#pragma once
#ifndef CAMERA_SETTINGS_H
#define CAMERA_SETTINGS_H

#include "Editor/editorpanel.h"
#include "Graphic/Rendering/camera.h"

#include <imgui.h>

class CameraSettings : public EditorPanel {
public:
    CameraSettings(Camera* camera) : EditorPanel(false, "Camera"), m_camera(camera) {}

    void render() override {
        ImGui::Begin("Camera", &m_open);
        ImGui::SliderFloat("Camera Speed", &m_camera->m_movementSpeed, 0.1f, 10.0f);
        ImGui::SliderFloat("Camera Sensitivity", &m_camera->m_mouseSensitivity, 0.1f, 2.0f);
        ImGui::End();
    }
private:
    Camera* m_camera;
};


#endif
