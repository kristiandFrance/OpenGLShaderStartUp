// Library Inlcudes
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>
#include "ShaderLoader.h"
#include "Hexagon.h"
#include "Quad.h"


//++++++++++++++++++++++++++
//		GLOBAL VARS

float pi = 3.1415926535f;

GLFWwindow* Window = nullptr;	// Window Var.
int Program_FixedTri;			// Program Var.
float CurrentTime{ 0 };
float PreviousTime;
float DeltaTime;
float ElapsedTime;
int CurrentFrame{ 0 };
glm::vec3 ColorJumping;
glm::vec3 Color = glm::vec3(1.0f, 0.0f, 0.0f);

// Create Quad Shape Class
Quad QuadShape( 1.0f );

// Create Hexagon Shape Class
Hexagon HexShape( 0.5f );

// Init Texture
GLuint Program_Texture;
GLuint ImageTexture;

//++++++++++++++++++++++++++




void InitialSetup()
{
	// Load the Image Data
	int ImageWidth;
	int ImageHeight;
	int ImageComponents;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* ImageData = stbi_load(	"Resources/Textures/zerotwojump.png",
											&ImageWidth, &ImageHeight, &ImageComponents, 0);

	Program_Texture = ShaderLoader::CreateProgram( "Resources/Shaders/Texture.vert",
													 "Resources/Shaders/Texture.frag");

	
	QuadShape.GenerateInfo();
	HexShape.GenerateInfo();

	// Create and bin a new texture var
	glGenTextures(1, &ImageTexture);
	glBindTexture(GL_TEXTURE_2D, ImageTexture);

	// Check how many components the loaded image has (RGBA or RGB)
	GLint LoadedComponents = (ImageComponents == 4) ? GL_RGBA : GL_RGB;

	// Populate the texture with the image data
	glTexImage2D(GL_TEXTURE_2D, 0, LoadedComponents, ImageWidth, ImageHeight, 0,
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

	


	// QUAD STUFFS
	//QuadRotationAngle += DeltaTime * 60;
	//QuadPosition.x = sin(CurrentTime);
	//QuadPosition.y = cos(CurrentTime);
	QuadShape.TranslationMat = glm::translate(glm::mat4(1.0f), QuadShape.Position);
	QuadShape.RotationMat = glm::rotate(glm::mat4(1.0f), glm::radians(QuadShape.RotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	QuadShape.ScaleMat = glm::scale(glm::mat4(1.0f), QuadShape.Scale/* + (sin(CurrentTime) / 2 + 0.5f)*/);
	QuadShape.ModelMat = QuadShape.TranslationMat * QuadShape.RotationMat * QuadShape.ScaleMat;

	// HEXAGON STUFFS
	
	auto dothestuff = [](float x) {
		return (4 / 2 * pi) * std::fabs(std::fmod((x - pi / 2), 2 * pi) - pi) - 1.0f;
		};

	//HexShape.RotationAngle -= 0.636619f / pi * DeltaTime * 285;
	HexShape.Position.x = sin(CurrentTime);
	HexShape.Position.y = cos(CurrentTime);
	HexShape.TranslationMat = glm::translate(glm::mat4(1.0f), HexShape.Position);
	HexShape.RotationMat = glm::rotate(glm::mat4(1.0f), glm::radians(HexShape.RotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));
	HexShape.ScaleMat = glm::scale(glm::mat4(1.0f), HexShape.Scale/* + (sin(CurrentTime) / 2 + 0.5f)*/);
	HexShape.ModelMat = HexShape.TranslationMat * HexShape.RotationMat * HexShape.ScaleMat;

	// Frame Indexing
	ElapsedTime += DeltaTime;
	if (ElapsedTime > 0.01)
	{
		CurrentFrame = (CurrentFrame < 52) ? CurrentFrame + 1 : 0;
		ElapsedTime = 0;
	}


	// Color Chnage When Jumping
	auto getRange = [=](float x) {
		x *= 5;
			if (std::fmod(x, 6.0f) <= 2) {
				return 1.0f;
			}
			else if (std::fmod(x, 6.0f) <= 3.0f) {
				return (3.0f - std::fmod(x, 6.0f));
			}
			else if (std::fmod(x, 6.0f) <= 5.0f) {
				return 0.0f;
			}
			else {
				return (std::fmod(x, 6.0f) - 5.0f);
			}
		};
	
	if (CurrentFrame == 1)
		ColorJumping = glm::vec3(getRange(CurrentTime + 2), getRange(CurrentTime), getRange(CurrentTime - 2));



}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	// Bind Assets
	glUseProgram(Program_Texture);


	glBindVertexArray(HexShape.getVAO());

	// Send CurrentFrame to shaders via uniform
	GLint CurrentFrameLoc = glGetUniformLocation(Program_Texture, "CurrentFrame");
	glUniform1i(CurrentFrameLoc, CurrentFrame);

	// Send ColorJumping to shaders via uniform
	GLint ColorLoc = glGetUniformLocation(Program_Texture, "Color");
	glUniform3fv(ColorLoc, 1, glm::value_ptr(Color));

	// Send ColorJumping to shaders via uniform
	GLint ColorJumpingLoc = glGetUniformLocation(Program_Texture, "ColorJumping");
	glUniform3fv(ColorJumpingLoc, 1, glm::value_ptr(ColorJumping));

	// Send Vars to shaders via "Uniform"
	GLint CurrentTimeLoc = glGetUniformLocation(Program_Texture, "CurrentTime");
	glUniform1f(CurrentTimeLoc, CurrentTime);

	// Send Model Matrix via Uniform
	GLint ModelMatLoc = glGetUniformLocation(Program_Texture, "ModelMat");
	glUniformMatrix4fv(ModelMatLoc, 1, GL_FALSE, glm::value_ptr(HexShape.ModelMat));

	// Bind Texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ImageTexture);
	glUniform1i(glGetUniformLocation(Program_Texture, "Texture0"), 0);

	// Setting the filtering and mipmap parameters for this texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	

	// Render the Hexagon using EBO
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);



	// Render the Quad
	glBindVertexArray(QuadShape.getVAO());
	// Send CurrentFrame to shaders via uniform
	CurrentFrameLoc = glGetUniformLocation(Program_Texture, "CurrentFrame");
	glUniform1i(CurrentFrameLoc, CurrentFrame);

	// Send ColorJumping to shaders via uniform
	ColorLoc = glGetUniformLocation(Program_Texture, "Color");
	glUniform3fv(ColorLoc, 1, glm::value_ptr(Color));

	// Send ColorJumping to shaders via uniform
	ColorJumpingLoc = glGetUniformLocation(Program_Texture, "ColorJumping");
	glUniform3fv(ColorJumpingLoc, 1, glm::value_ptr(ColorJumping));

	// Send Vars to shaders via "Uniform"
	CurrentTimeLoc = glGetUniformLocation(Program_Texture, "CurrentTime");
	glUniform1f(CurrentTimeLoc, CurrentTime);

	// Send Model Matrix via Uniform
	ModelMatLoc = glGetUniformLocation(Program_Texture, "ModelMat");
	glUniformMatrix4fv(ModelMatLoc, 1, GL_FALSE, glm::value_ptr(QuadShape.ModelMat));

	// Bind Texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ImageTexture);
	glUniform1i(glGetUniformLocation(Program_Texture, "Texture0"), 0);

	// Setting the filtering and mipmap parameters for this texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

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
	Window = glfwCreateWindow(800, 800, "I wonder if the 3rd story is high enough..", NULL, NULL);
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


