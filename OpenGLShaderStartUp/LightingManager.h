#pragma once

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include "LightColorSpheres.h"
#include "Camera.h"

struct PointLight
{
    glm::vec3 Position;
    glm::vec3 Color;
    float SpecularStrength;
};

class LightingManager
{
public:
    LightingManager(Camera* _CameraRef);

    void Update(GLFWwindow* Window);
    void Render(GLuint* Program, GLuint* SphereProgram);

    LightColorSpheres* LightSphere;

private:
    bool PointLightPreviousKeyState = GLFW_RELEASE;
    bool PointLightToggleState = false;
    int PointLightEnabled = 0;

    bool DirectionalLightPreviousKeyState = GLFW_RELEASE;
    bool DirectionalLightToggleState = false;
    int DirectionalLightEnabled = 0;

    bool SpotlightPreviousKeyState = GLFW_RELEASE;
    bool SpotlightToggleState = false;
    int SpotlightEnabled = 0;

    static const int MAX_POINT_LIGHTS = 4;
    PointLight PointLightArray[MAX_POINT_LIGHTS];
    unsigned int PointLightCount = 2; // Assume 2 point lights.
};

