#pragma once
#include "Shape.h"

class Camera
{
public:




	glm::mat4 ProjectionMat;
	glm::mat4 ViewMat;
	glm::vec3 CameraPos = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 CameraLookDir;
	glm::vec3 CameraTargetPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 CameraUpDir = glm::vec3(0.0f, 1.0f, 0.0f);
};

