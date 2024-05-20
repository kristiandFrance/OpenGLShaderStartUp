// Library Inlcudes
#include <iostream>
#include "ShaderLoader.h"
#include "Hexagon.h"
#include "Quad.h"
#include "Model.h"
#include "Camera.h"


//++++++++++++++++++++++++++
//		GLOBAL VARS

GLFWwindow* Window = nullptr;	// Window Var.
int WindowHeight = 800;			// Window Height.
int WindowWidth = 800;			// Window Width.
int Program_FixedTri;			// Program Var.
float CurrentTime{ 0 };
float PreviousTime;
float DeltaTime;


// Object Stuffs
Model* SwordModel;
Model* StatueModel;
Camera CameraObj;


// Init Program
GLuint Program_3DShader;


//++++++++++++++++++++++++++




void InitialSetup()
{
	// Create Model Classes
	StatueModel = new Model("Resources/Models/SM_Prop_Statue_01.obj", &CameraObj, 1,
							"Resources/Textures/PolygonAncientWorlds_Statue_01.png",
							0.0f);

	SwordModel = new Model(	"Resources/Models/Sword.obj", &CameraObj, 1000, 
							"Resources/Textures/SwordTex.jpg",
							90.0f);


	// 3D Program Shader
	Program_3DShader = ShaderLoader::CreateProgram(	"Resources/Shaders/Texture.vert",
													"Resources/Shaders/Texture.frag");

	// Back Face Culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);


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


	// Update Objects/Models
	CameraObj.Update(DeltaTime, CurrentTime);

	SwordModel->Update(DeltaTime);
	StatueModel->Update(DeltaTime);



}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render Models
	SwordModel->Render(&Program_3DShader);
	StatueModel->Render(&Program_3DShader);

	// Unbind Assets
	glBindVertexArray(0);
	glUseProgram(0);


	glfwSwapBuffers(Window);
}



int main()
{


	// Initialising GLFW and setting the version to 4.6 with core profile
	glfwInit();
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	

	// Create an GLFW controlled context window
	Window = glfwCreateWindow(WindowWidth, WindowHeight, "Do shotguns generally taste good???", NULL, NULL);
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


