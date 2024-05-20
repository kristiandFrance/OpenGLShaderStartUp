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


struct VertexStandard
{
	glm::vec3 position;
	glm::vec2 texcoord;
	//glm::vec3 normal;

	VertexStandard()
	{
		position = glm::vec3(0.0f);
		texcoord = glm::vec2(0.0f);
		//normal	 = glm::vec3(0.0f);
	};
	VertexStandard(glm::vec3 pos, glm::vec2 texc/*, glm::vec3 norm*/)
	{
		position = pos;
		texcoord = texc;
		//normal   = norm;
	}
};

class Model
{
public:
	Model(std::string FilePath, Camera* _CameraRef, int _InstanceCount, std::string TextureFilePath, float _RotationAngle);
	~Model();

	virtual void Update(float DeltaTime);
	virtual void Render(GLuint* Program);


	glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 TranslationMat;

	float RotationAngle;
	glm::mat4 RotationMat;

	glm::vec3 Scale = glm::vec3(0.02f);
	glm::mat4 ScaleMat;

	glm::mat4 ModelMat;



protected:
	// Init Texture
	GLuint Texture;
	Camera* CameraRef;
	GLuint VAO;
	GLuint DrawCount;
	GLint InstanceCount;
	glm::mat4* InstancedModels;
	int DrawType;
	glm::mat4 VPMatrix;


	
};

