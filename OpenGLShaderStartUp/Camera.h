#pragma once

// Library Includes
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include "ShaderLoader.h"

class Camera
{
public:
    Camera();
    ~Camera();

    void Update(float DeltaTime, float CurrentTime, GLFWwindow* _WindowRef);

    glm::mat4 GetViewMat();
    glm::mat4 GetProjMat();
	glm::vec3 GetCameraPos();
    glm::vec3 GetCameraTargetPos();

    float FOV = 103.0f;
    float MinFOV = 10.0f;
    float MaxFOV = 103.0f;

protected:
    float Distance = 4;
    float AspectRatio = 1;
    float Yaw;
    float Pitch;
    float LastMouseX;
    float LastMouseY;
    bool FirstMouse = true;
    float MouseSensitivity = 0.1f;
	bool HideMouse = false;
    bool InvertMouse = false;

    glm::mat4 ProjectionMat;
    glm::mat4 ViewMat;

    glm::vec3 CameraPos = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 CameraUpDir = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 CameraTargetPos = glm::vec3(0.0f, 0.0f, 0.0f);


};