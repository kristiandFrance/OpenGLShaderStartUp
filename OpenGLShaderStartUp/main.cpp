// Library Inlcudes
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>
#include "ShaderLoader.h"



//++++++++++++++++++++++++++
//		GLOBAL VARS

float pi = 3.1415926535f;

GLFWwindow* Window = nullptr;	// Window Var.
int Program_FixedTri;			// Program Var.
float CurrentTime;
float PreviousTime;
float DeltaTime;

// Quad2 Tri Verts
GLfloat Vertices_Indexed_Quad[] = {
	// Index	// Postition			// Colour			// Texture Coords	
	/* 0 */		-0.5f,  0.5f,  0.0f,	1.0f, 0.0f, 0.0f,	-2.0f,  2.0f,			// Top Left
	/* 1 */		-0.5f, -0.5f,  0.0f,	0.0f, 1.0f, 0.0f,	-2.0f, -2.0f,			// Bottom Left
	/* 2 */		 0.5f, -0.5f,  0.0f,	1.0f, 0.0f, 1.0f,	 2.0f, -2.0f,			// Bottom Right
	/* 3 */		 0.5f,  0.5f,  0.0f,	0.0f, 1.0f, 1.0f,	 2.0f,  2.0f,			// Top Right
};

GLuint Program_Texture;

// Creating VBO/VAO and Indices for Quad EBO
GLuint VAO_Indexed_Quad;
GLuint EBO_Indexed_Quad;
GLuint VBO_Indexed_Quad;
GLuint Indices_Quad[] = {
	0, 1, 2,	// First Tri   ( TL -> BL -> BR )
	0, 2, 3,	// Second Tri  ( TL -> BR -> TR )
};

// Object Matrices and Components
glm::vec3 QuadPosition = glm::vec3(0.0f, 0.0f, 0.0f);
glm::mat4 TranslationMat;

float QuadRotationAngle = 0.0f;
glm::mat4 RotationMat;

glm::vec3 QuadScale = glm::vec3(1.5f, 1.5f, 1.0f);
glm::mat4 ScaleMat;

glm::mat4 QuadModelMat;

GLuint AwesomeFaceTexture;

//++++++++++++++++++++++++++




void InitialSetup()
{
	// Load the Image Data
	int ImageWidth;
	int ImageHeight;
	int ImageComponents;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* ImageData = stbi_load(	"Resources/Textures/zerotwo.png",
											&ImageWidth, &ImageHeight, &ImageComponents, 0);

	Program_Texture = ShaderLoader::CreateProgram( "Resources/Shaders/Texture.vert",
													 "Resources/Shaders/Texture.frag");

	

	// Generate VAO For Quad
	glGenVertexArrays(1, &VAO_Indexed_Quad);
	glBindVertexArray(VAO_Indexed_Quad);

	// Generate EBO For Quad
	glGenBuffers(1, &EBO_Indexed_Quad);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_Indexed_Quad);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices_Quad), Indices_Quad, GL_STATIC_DRAW);

	// Generate VBO For Quad
	glGenBuffers(1, &VBO_Indexed_Quad);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Indexed_Quad);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices_Indexed_Quad), Vertices_Indexed_Quad, GL_STATIC_DRAW);


	// Set the Vertex Attribute Information
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Set the Vertex Color Information
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Set the Texture Information
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// Create and bin a new texture var
	glGenTextures(1, &AwesomeFaceTexture);
	glBindTexture(GL_TEXTURE_2D, AwesomeFaceTexture);

	// Check how many components the loaded image has (RGBA or RGB)
	GLint LoadedComponents = (ImageComponents == 4) ? GL_RGBA : GL_RGB;

	// Populate the texture with the image data
	glTexImage2D(	GL_TEXTURE_2D, 0, LoadedComponents, ImageWidth, ImageHeight, 0,
					LoadedComponents, GL_UNSIGNED_BYTE, ImageData);

	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(ImageData);
	glBindTexture(GL_TEXTURE_2D, 0);


	// Set the colour of the window for when the buffer is cleared
	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);

	// Maps the range of the window size to NDC (-1 -> 1)
	glViewport(0, 0, 800, 800);
}

void Update()
{
	glfwPollEvents();

	// Get Current Time
	PreviousTime = CurrentTime;
	CurrentTime = glfwGetTime();
	DeltaTime = CurrentTime - PreviousTime;

	//QuadRotationAngle += DeltaTime * (1 / DeltaTime);


	// Calculate the Model Matrix
	//QuadPosition.x = sin(CurrentTime);
	//QuadPosition.y = sin(CurrentTime - (pi / 2.0f));

	TranslationMat = glm::translate(glm::mat4(1.0f), QuadPosition);
	RotationMat = glm::rotate(glm::mat4(1.0f), glm::radians(QuadRotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	ScaleMat = glm::scale(glm::mat4(1.0f), QuadScale/* + (sin(CurrentTime) / 2 + 0.5f)*/);
	QuadModelMat = TranslationMat * RotationMat * ScaleMat;

}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	// Bind Assets
	glUseProgram(Program_Texture);
	glBindVertexArray(VAO_Indexed_Quad);	

	// Send Vars to shaders via "Uniform"
	GLint CurrentTimeLoc = glGetUniformLocation(Program_Texture, "CurrentTime");
	glUniform1f(CurrentTimeLoc, CurrentTime);

	// Send Model Matrix via Uniform
	GLint ModelMatLoc = glGetUniformLocation(Program_Texture, "QuadModelMat");
	glUniformMatrix4fv(ModelMatLoc, 1, GL_FALSE, glm::value_ptr(QuadModelMat));

	// Bind Texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, AwesomeFaceTexture);
	glUniform1i(glGetUniformLocation(Program_Texture, "Texture0"), 0);

	// Setting the filtering and mipmap parameters for this texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Render the Quad using EBO
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


	// Unbind Assets
	glBindVertexArray(0);
	glUseProgram(0);



	glfwSwapBuffers(Window);
}



int main()
{
	// Initialising GLFW and setting the version to 4.6 with compatability profile
	glfwInit();
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	// Create an GLFW controlled context window
	Window = glfwCreateWindow(800, 800, "FML, I WANT TO DIE HAHAHAHHAHAHA!", NULL, NULL);
	if (Window == NULL)
	{
		std::cout << "GLFW failed to init. properly. Terminating program." << std::endl;
		system("pause");

		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(Window);


	// Init. GLEW to populate OpenGL function pointers
	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW failed to init. properly. Terminating program." << std::endl;
		system("pause");

		glfwTerminate();
		return -1;
	}


	// Setup the Initial elements of the program
	InitialSetup();
	



	while (glfwWindowShouldClose(Window) == false)
	{
		// Update all object and run the processes
		Update();

		// Render all the objects
		Render();
	}

	// Ensuring correct shutdown of GLFW
	glfwTerminate();
	return 0;



}


