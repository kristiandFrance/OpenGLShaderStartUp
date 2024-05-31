#include "Camera.h"

GLuint Program_ClipSpace;


Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::Update(float DeltaTime, float CurrentTime, GLFWwindow* _WindowRef)
{
    if (glfwGetKey(_WindowRef, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        RotationSpeed = 1.0f;
    }
    else if (glfwGetKey(_WindowRef, GLFW_KEY_CAPS_LOCK) == GLFW_PRESS)
    {
        RotationSpeed = 0.0f;
    }
    else { RotationSpeed = 0.5f; }

    RotationAngle += DeltaTime * RotationSpeed;


    CameraPos.x = Distance * sin(RotationAngle);
    CameraPos.z = Distance * cos(RotationAngle);

    // Calculate the Projection Matrix (Anchor Point = (0, 0))
    ProjectionMat = glm::perspective(glm::radians(FOV), AspectRatio, 0.1f, 100.0f);

    ViewMat = glm::lookAt(CameraPos, CameraTargetPos, CameraUpDir);
}

glm::mat4 Camera::GetViewMat()
{
    return ViewMat;
}

glm::mat4 Camera::GetProjMat()
{
    return ProjectionMat;
}

glm::vec3 Camera::GetCameraPos()
{
    return CameraPos;
}

glm::vec3 Camera::GetCameraTargetPos()
{
    return CameraTargetPos;
}