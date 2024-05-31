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
	Model(	std::string FilePath, Camera* _CameraRef, int _InstanceCount,
			std::string TextureFilePath, float _RotationAngle, float _scale, 
			float _VerticalOffset, bool _RandVariance);
	~Model();

	virtual void Update(float DeltaTime, GLFWwindow* _WindowRef);
	virtual void Render(GLuint* Program);
	virtual void ChangeTexture(std::string NewTexture);
	void ToggleControl();

	



protected:
	void ClearModelData();
	void ProcessInput(float DeltaTime, GLFWwindow* _WindowRef);


	bool RandVariance;

	float VerticalOffset;
	glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 TranslationMat;

	float RotationAngle;
	glm::mat4 RotationMat;

	glm::vec3 Scale;
	glm::mat4 ScaleMat;

	glm::mat4 ModelMat;


	bool Controlling = false;

	// Init Texture
	GLuint Texture;
	Camera* CameraRef;
	GLuint VAO;
	GLuint VBO;
	GLuint InstanceVBO;
	GLuint DrawCount;
	GLint InstanceCount;
	glm::mat4* InstancedModels;
	int DrawType;
	glm::mat4 VPMatrix;


	
};

