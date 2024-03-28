#pragma once
#include <vector>
#include <cmath>
#include <iostream>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

struct vertex {
	float x, y, z;
};

class Shape
{
public:

	GLuint& getVAO();
	GLuint& getEBO();
	GLuint& getVBO();


	glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 TranslationMat;

	float RotationAngle = 0.0f;
	glm::mat4 RotationMat;

	glm::vec3 Scale = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::mat4 ScaleMat;

	glm::mat4 ModelMat;


protected:
	
private:

	GLuint VAO;
	GLuint EBO;
	GLuint VBO;


};

