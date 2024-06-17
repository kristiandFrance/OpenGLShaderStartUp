#include "LightingManager.h"

LightingManager::LightingManager(Camera* _CameraRef)
{
    // Initialize the first point light (Blue light)
    PointLightArray[0].Position = glm::vec3(0.0f, 1.0f, 2.0f); // Adjust position as needed
    PointLightArray[0].Color = glm::vec3(0.0f, 0.0f, 1.0f); // Blue light
    PointLightArray[0].SpecularStrength = 1.0f;

    // Initialize the second point light (Red light)
    PointLightArray[1].Position = glm::vec3(2.0f, 1.0f, 0.0f); // Adjust position as needed
    PointLightArray[1].Color = glm::vec3(1.0f, 0.0f, 0.0f); // Red light
    PointLightArray[1].SpecularStrength = 1.0f;

    PointLightCount = 2; // Number of point lights

    glm::vec3 Positions[2] = { PointLightArray[0].Position, PointLightArray[1].Position };
    glm::vec3 Colors[2] = { PointLightArray[0].Color, PointLightArray[1].Color };

    LightSphere = new LightColorSpheres("Resources/Models/Sphere_HighPoly.obj", _CameraRef, 2, 0.25f, Positions, Colors);

}

void LightingManager::Update(GLFWwindow* Window)
{
    // Toggle point lights with a key press if needed
    if (glfwGetKey(Window, GLFW_KEY_1) == GLFW_PRESS && PointLightPreviousKeyState == GLFW_RELEASE)
    {
        PointLightToggleState = !PointLightToggleState;
        PointLightEnabled = PointLightToggleState ? 1 : 0;
    }
    PointLightPreviousKeyState = glfwGetKey(Window, GLFW_KEY_1);

	LightSphere->Update(Window);
}

void LightingManager::Render(GLuint* Program, GLuint* SphereProgram)
{
    glUseProgram(*Program);
    
    for (int i = 0; i < PointLightCount; ++i)
    {
        std::string indexStr = std::to_string(i);
        glUniform3fv(glGetUniformLocation(*Program, ("PointLightArray[" + indexStr + "].Position").c_str()), 1, glm::value_ptr(PointLightArray[i].Position));
        glUniform3fv(glGetUniformLocation(*Program, ("PointLightArray[" + indexStr + "].Color").c_str()), 1, glm::value_ptr(PointLightArray[i].Color));
        glUniform1f(glGetUniformLocation(*Program, ("PointLightArray[" + indexStr + "].SpecularStrength").c_str()), PointLightArray[i].SpecularStrength);
    }

    glUniform1ui(glGetUniformLocation(*Program, "PointLightCount"), PointLightCount);
    glUseProgram(0);

    LightSphere->Render(SphereProgram);
}
