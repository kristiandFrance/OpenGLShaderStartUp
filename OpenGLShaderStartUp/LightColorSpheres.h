#pragma once

#include <vector>
#include <cmath>
#include <iostream>
#include <array>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Camera.h"
#include "ShaderLoader.h"


struct VertexStandardSolidColor
{
	glm::vec3 position;
	glm::vec2 texcoord;

    VertexStandardSolidColor()
	{
		position = glm::vec3(0.0f);
		texcoord = glm::vec2(0.0f);
	};
    VertexStandardSolidColor(glm::vec3 pos, glm::vec2 texc/*, glm::vec3 norm*/)
	{
		position = pos;
		texcoord = texc;
	}
};

class LightColorSpheres
{
public:
    LightColorSpheres(std::string ObjectFilePath, Camera* _CameraRef, int _InstanceCount,
        float _scale, glm::vec3* _Position, glm::vec3* _Colors);
    ~LightColorSpheres();

    virtual void Update(GLFWwindow* _WindowRef);
    virtual void Render(GLuint* _Program);

private:
    glm::vec3 Position;
    glm::vec3* Colors;  // Array to store colors
    glm::mat4 TranslationMat;
    glm::mat4 RotationMat;
    glm::vec3 Scale;
    glm::mat4 ScaleMat;
    glm::mat4 LightColorSpheresMat;
    Camera* CameraRef;
    GLuint VAO;
    GLuint VBO;
    GLuint InstanceVBO;
    GLuint ColorVBO;  // New VBO for colors
    GLuint DrawCount;
    GLint InstanceCount;
    glm::mat4* InstancedLightColorSpheress;
    int DrawType;
    glm::mat4 VPMatrix;
};
