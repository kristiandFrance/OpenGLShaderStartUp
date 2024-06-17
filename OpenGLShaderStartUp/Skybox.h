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

class Skybox
{
public:
    Skybox(Camera* _CameraRef, std::string FilePaths[]);
    ~Skybox() = default;

    void Render();
    GLuint TextureID_Skybox;


private:
    Camera* CameraRef;
    GLuint VAO;
    GLuint VBO;
    GLuint DrawCount;
    GLuint Program;
};
