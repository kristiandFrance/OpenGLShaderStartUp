#include "Model.h"

// Library Defines
#define TINYOBJLOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

// Library Includes
#include <tiny_obj_loader.h>
#include <stb_image.h>
#include <iostream>
#include <random>

Model::Model(	std::string ObjectFilePath, Camera* _CameraRef, int _InstanceCount,
				std::string TextureFilePath, float _RotationAngle, float _scale, 
				float _VerticalOffset, bool _RandVariance)
{
	RotationAngle = _RotationAngle;
	Scale = glm::vec3(_scale);
	VerticalOffset = _VerticalOffset;
	RandVariance = _RandVariance;
	InstanceCount = _InstanceCount;
	CameraRef = _CameraRef;
	InstancedModels = new glm::mat4[InstanceCount];

	ChangeTexture(TextureFilePath);

	std::vector<VertexStandard> Vertices;
	tinyobj::ObjReaderConfig ReaderConfig;
	tinyobj::ObjReader Reader;

	if (!Reader.ParseFromFile(ObjectFilePath, ReaderConfig))
	{
		if (!Reader.Error().empty()) 
		{
			std::cerr << "TinyObjReader Error: " << Reader.Error() << std::endl;
		}
		exit(1);
	}

	if (!Reader.Warning().empty())
	{
		std::cout << "TinyObjReader Warning: " << Reader.Warning() << std::endl;
	}

	auto& Attrib = Reader.GetAttrib();
	auto& Shapes = Reader.GetShapes();

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(0.0f, 1.0f);

	for (int i = 0; i < InstanceCount; i++)
	{
		// Align Instances into a grid and centre it
		Position = glm::vec3(	std::fmod(i, std::sqrt(InstanceCount)) - std::sqrt(InstanceCount) / 2.0f,					// x
								0.0f + VerticalOffset, 																						// y
								float(floor(i / std::sqrt(InstanceCount))) - std::sqrt(InstanceCount) / 2.0f);				// z
		glm::vec3 RotationApplyVector = glm::vec3(1.0f, 0.0f, 0.0f);

		if (RandVariance)
		{
			// Give Randomness If Requested
			Position += glm::vec3(	dis(gen) * 0.3,			// x
									dis(gen) * 0.1, 		// y
									dis(gen) * 0.3);		// z

			RotationApplyVector += glm::vec3(	-0.05f + dis(gen) * 0.1,        // x
												-0.2f + dis(gen) * 0.4,         // y
												-0.2f + dis(gen) * 0.4);        // z
		}


		// Matrix Application
		TranslationMat = glm::translate(glm::mat4(1.0f), Position);
		RotationMat = glm::rotate(glm::mat4(1.0f), glm::radians(RotationAngle), RotationApplyVector);
		ScaleMat = glm::scale(glm::mat4(1.0f), Scale);
		ModelMat = TranslationMat * RotationMat * ScaleMat;

		InstancedModels[i] = (ModelMat);
	}

	//ClearModelData();

	// Loading The Model
	//=================================================
	
	// Loop through the shapes of the object
	for (size_t ShapeIndex = 0; ShapeIndex < Shapes.size(); ShapeIndex++)
	{
		// Loop through the faces of the shape
		size_t ReadIndexOffset = 0;
		for (size_t FaceIndex = 0; FaceIndex < Shapes[ShapeIndex].mesh.num_face_vertices.size(); FaceIndex++)
		{
			size_t FaceVertexCount = size_t(Shapes[ShapeIndex].mesh.num_face_vertices[FaceIndex]);
			// Loop through the vertices of the face
			for (size_t VertexIndex = 0; VertexIndex < FaceVertexCount; VertexIndex++)
			{
				VertexStandard Vertex{};
				tinyobj::index_t TinyObjVertex = Shapes[ShapeIndex].mesh.indices[ReadIndexOffset + VertexIndex];
				Vertex.position = {
					Attrib.vertices[3 * size_t(TinyObjVertex.vertex_index) + 0],
					Attrib.vertices[3 * size_t(TinyObjVertex.vertex_index) + 1],
					Attrib.vertices[3 * size_t(TinyObjVertex.vertex_index) + 2],
				};
				if (TinyObjVertex.texcoord_index >= 0) // Negative states no TexCoord data
				{
					Vertex.texcoord = {
						Attrib.texcoords[2 * size_t(TinyObjVertex.texcoord_index) + 0],
						Attrib.texcoords[2 * size_t(TinyObjVertex.texcoord_index) + 1],
					};
				}

				/*if (TinyObjVertex.normal_index >= 0) // Negative states no Normal data
				{
					Vertex.normal = {
						Attrib.normals[3 * size_t(TinyObjVertex.normal_index) + 0],
						Attrib.normals[3 * size_t(TinyObjVertex.normal_index) + 1],
						Attrib.normals[3 * size_t(TinyObjVertex.normal_index) + 2],
					};
				}*/
				Vertices.push_back(Vertex);
			}
			ReadIndexOffset += FaceVertexCount;
		}
	}
	

	// Store for use in Rendering
	DrawType = GL_TRIANGLES;
	DrawCount = (GLuint)Vertices.size();

	// Create the Vertex Array and associated buffers

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &InstanceVBO);

	glBindVertexArray(VAO);


	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexStandard) * DrawCount, Vertices.data(), GL_STATIC_DRAW);

	// Create the VertexAttribPoionters for Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexStandard), (void*)0);

	// Create the VertexAttribPointer for Texture Coords
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexStandard), (void*)offsetof(VertexStandard, texcoord));


	glBindBuffer(GL_ARRAY_BUFFER, InstanceVBO);
	glBufferData(GL_ARRAY_BUFFER, InstanceCount * sizeof(glm::mat4), InstancedModels, GL_DYNAMIC_DRAW);

	// Create the VertexAttribPointer for Instanced MVP
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
	glVertexAttribDivisor(2, 1);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(1 * sizeof(glm::vec4)));
	glVertexAttribDivisor(3, 1);

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
	glVertexAttribDivisor(4, 1);

	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
	glVertexAttribDivisor(5, 1);



	glBindVertexArray(0);

	

	// Setting the filtering and mipmap parameters for this texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);


}

Model::~Model()
{
}

void Model::Update(float DeltaTime, GLFWwindow* _WindowRef)
{
	
	VPMatrix = CameraRef->GetProjMat() * CameraRef->GetViewMat();
	
	if (Controlling)
	{
		ProcessInput(DeltaTime, _WindowRef);

		for (int i = 0; i < InstanceCount; i++)
		{
			TranslationMat = glm::translate(glm::mat4(1.0f), Position);
			RotationMat = glm::rotate(glm::mat4(1.0f), glm::radians(RotationAngle), glm::vec3(1.0f, 0.0f, 0.0f));
			ScaleMat = glm::scale(glm::mat4(1.0f), Scale);
			ModelMat = TranslationMat * RotationMat * ScaleMat;

			InstancedModels[i] = (ModelMat);
		}
		std::cout << Position.x << ", " << Position.y << ", " << Position.z << "\n";

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, InstanceVBO);
		glBufferData(GL_ARRAY_BUFFER, InstanceCount * sizeof(glm::mat4), InstancedModels, GL_DYNAMIC_DRAW);

		glBindVertexArray(0);
	}

	

	
	
}

void Model::Render(GLuint* Program)
{

	glUseProgram(*Program);
	glBindVertexArray(VAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);

	glUniform1i(glGetUniformLocation(*Program, "Texture0"), 0);

	glUniformMatrix4fv(glGetUniformLocation(*Program, "VP"), 1, GL_FALSE, glm::value_ptr(VPMatrix));

	glDrawArraysInstanced(DrawType, 0, DrawCount, InstanceCount);


	// Unbind Assets
	glBindVertexArray(0);
	glUseProgram(0);

}

void Model::ChangeTexture(std::string NewTexture)
{


	// Load the Image Data
	int ImageWidth;
	int ImageHeight;
	int ImageComponents;

	unsigned char* ImageData = stbi_load(NewTexture.c_str(),
		&ImageWidth, &ImageHeight, &ImageComponents, 0);

	// Create and Bind a new texture var
	//glDeleteTextures(1, &Texture);
	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);

	// check how many components he loaded image has
	GLint LoadedComponents = (ImageComponents == 4) ? GL_RGBA : GL_RGB;

	// Populate the texture with the image data
	glTexImage2D(GL_TEXTURE_2D, 0, LoadedComponents, ImageWidth, ImageHeight, 0,
		LoadedComponents, GL_UNSIGNED_BYTE, ImageData);

	// Generate the mipmaps, free the memory and unbind the texture
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(ImageData);
	glBindTexture(GL_TEXTURE_2D, 0);

}

void Model::ToggleControl()
{
	Controlling = !Controlling;
}

void Model::ClearModelData()
{
	Position = glm::vec3(0.0f, 0.0f, 0.0f);
	RotationAngle = 0.0f;
	Scale = glm::vec3(0.0f);
}

void Model::ProcessInput(float DeltaTime, GLFWwindow* _WindowRef)
{
	float Magnitude = 10.0f;
	glm::vec3 CameraVec = glm::normalize(CameraRef->GetCameraTargetPos() - CameraRef->GetCameraPos());

	if (glfwGetKey(_WindowRef, GLFW_KEY_W) == GLFW_PRESS)
	{
		Position += glm::vec3(DeltaTime * Magnitude) * glm::vec3(1.0f, 0.0f, 1.0f) * CameraVec;
	}
	if (glfwGetKey(_WindowRef, GLFW_KEY_S) == GLFW_PRESS)
	{
		Position += glm::vec3(DeltaTime * Magnitude) * glm::vec3(-1.0f, 0.0f, -1.0f) * CameraVec;
	}
	if (glfwGetKey(_WindowRef, GLFW_KEY_Q) == GLFW_PRESS)
	{
		Position += glm::vec3(DeltaTime * Magnitude) * glm::vec3(0.0f, -1.0f, 0.0f);
	}
	if (glfwGetKey(_WindowRef, GLFW_KEY_E) == GLFW_PRESS)
	{
		Position += glm::vec3(DeltaTime * Magnitude) * glm::vec3(0.0f, 1.0f, 0.0f);
	}
	if (glfwGetKey(_WindowRef, GLFW_KEY_A) == GLFW_PRESS)
	{
		glm::vec3 LeftVec;
		LeftVec.x = CameraVec.z;
		LeftVec.y = CameraVec.y;
		LeftVec.z = -CameraVec.x;

		Position += glm::vec3(DeltaTime * Magnitude) * glm::vec3(1.0f, 0.0f, 1.0f) * LeftVec;
	}
	if (glfwGetKey(_WindowRef, GLFW_KEY_D) == GLFW_PRESS)
	{
		glm::vec3 RightVec;
		RightVec.x = -CameraVec.z;
		RightVec.y = -CameraVec.y;
		RightVec.z = CameraVec.x;

		Position += glm::vec3(DeltaTime * Magnitude) * glm::vec3(1.0f, 0.0f, 1.0f) * RightVec;
	}
	
	
}
