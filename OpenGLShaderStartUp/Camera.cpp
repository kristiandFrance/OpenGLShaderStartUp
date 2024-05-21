#include "Camera.h"

GLuint Program_ClipSpace;


Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::Update(float DeltaTime, float CurrentTime)
{

    CameraPos.x = Distance * sin(CurrentTime * 2);
	CameraPos.z = Distance * cos(CurrentTime * 2);

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