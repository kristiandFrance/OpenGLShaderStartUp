#pragma once

// Library Includes
#define STB_IMAGE_IMPLEMENTATION
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

    void Update(float DeltaTime, float CurrentTime);


    glm::mat4 GetViewMat();
    glm::mat4 GetProjMat();

protected:
    float Distance = 4;
    float FOV = 103.0f;
    float AspectRatio = 1;

    glm::mat4 ProjectionMat;
    glm::mat4 ViewMat;

    glm::vec3 CameraPos = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 CameraUpDir = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 CameraTargetPos = glm::vec3(0.0f, 0.0f, 0.0f);


};