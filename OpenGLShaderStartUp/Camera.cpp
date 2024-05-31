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
	//													Movement Logic
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	// Pressing LCTRL "Sprints"

	float Magnitude = 8.0f;
    if (glfwGetKey(_WindowRef, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
		Magnitude = 15.0f;
    }


    // Camera WASD Movement ( Space and LShift keys for Up and Down )

	glm::vec3 CameraVec = glm::normalize(GetCameraTargetPos() - GetCameraPos());
	glm::vec3 ChangeVec;

	if (glfwGetKey(_WindowRef, GLFW_KEY_W) == GLFW_PRESS)
	{
		ChangeVec = glm::vec3(DeltaTime * Magnitude) * glm::vec3(1.0f, 0.0f, 1.0f) * CameraVec;

		CameraPos += ChangeVec;
		CameraTargetPos += ChangeVec;
	}
	if (glfwGetKey(_WindowRef, GLFW_KEY_S) == GLFW_PRESS)
	{
		ChangeVec = glm::vec3(DeltaTime * Magnitude) * glm::vec3(-1.0f, 0.0f, -1.0f) * CameraVec;

		CameraPos += ChangeVec;
		CameraTargetPos += ChangeVec;
	}
	if (glfwGetKey(_WindowRef, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		ChangeVec = glm::vec3(DeltaTime * Magnitude) * -CameraUpDir;

		CameraPos += ChangeVec;
		CameraTargetPos += ChangeVec;
	}
	if (glfwGetKey(_WindowRef, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		ChangeVec = glm::vec3(DeltaTime * Magnitude) * CameraUpDir;

		CameraPos += ChangeVec;
		CameraTargetPos += ChangeVec;
	}
	if (glfwGetKey(_WindowRef, GLFW_KEY_A) == GLFW_PRESS)
	{
		glm::vec3 LeftVec;
		LeftVec.x = CameraVec.z;
		LeftVec.y = CameraVec.y;
		LeftVec.z = -CameraVec.x;

		ChangeVec = glm::vec3(DeltaTime * Magnitude) * glm::vec3(1.0f, 0.0f, 1.0f) * LeftVec;

		CameraPos += ChangeVec;
		CameraTargetPos += ChangeVec;
	}
	if (glfwGetKey(_WindowRef, GLFW_KEY_D) == GLFW_PRESS)
	{
		glm::vec3 RightVec;
		RightVec.x = -CameraVec.z;
		RightVec.y = -CameraVec.y;
		RightVec.z = CameraVec.x;

		ChangeVec = glm::vec3(DeltaTime * Magnitude) * glm::vec3(1.0f, 0.0f, 1.0f) * RightVec;

		CameraPos += ChangeVec;
		CameraTargetPos += ChangeVec;
	}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~




	//													Camera Mouse Look Direction
	//================================================================================================================================

	
	// PUT IN SENSITIVITY CONTROLS HERE SOMETIME TY FUTURE ME


	double MouseX, MouseY;
	glfwGetCursorPos(_WindowRef, &MouseX, &MouseY);

	if (FirstMouse)
	{
		LastMouseX = MouseX;
		LastMouseY = MouseY;
		FirstMouse = false;
	}

	float xOffset = (LastMouseX - MouseX) * MouseSensitivity;
	
	float yOffset = InvertMouse ? ((MouseY - LastMouseY) * MouseSensitivity) : ((LastMouseY - MouseY) * MouseSensitivity);

	LastMouseX = MouseX;
	LastMouseY = MouseY;

	Yaw += xOffset;
	Pitch += yOffset;


	// Limit the angle to prevent flipping

	if (Pitch > 89.9f)
	{
		Pitch = 89.9f;
	}
	if (Pitch < -89.9f)
	{
		Pitch = -89.9f;
	}


	// Calculate the new camera direction using quaternions

	glm::quat QuaternionPitch = glm::angleAxis(glm::radians(Pitch), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::quat QuaternionYaw = glm::angleAxis(glm::radians(Yaw), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::quat Orientation = QuaternionYaw * QuaternionPitch;
	Orientation = glm::normalize(Orientation);

	glm::vec3 Front = Orientation * glm::vec3(0.0f, 0.0f, -1.0f);
	CameraTargetPos = CameraPos + Front;

	//================================================================================================================================


    // Calculate the Projection Matrix 

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