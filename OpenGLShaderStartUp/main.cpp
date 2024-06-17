// Library Inlcudes
#include <iostream>
#include "ShaderLoader.h"
#include "Hexagon.h"
#include "Quad.h"
#include "Model.h"
#include "Camera.h"
#include "Skybox.h"
#include "LightingManager.h"


//++++++++++++++++++++++++++
//		GLOBAL VARS

GLFWwindow* Window = nullptr;	// Window Var.
int WindowHeight = 800;			// Window Height.
int WindowWidth = 800;			// Window Width.
float CurrentTime{ 0 };
float PreviousTime;
float DeltaTime;
bool MeshPreviousKeyState = GLFW_RELEASE;
bool MeshToggleState = false;


// Object Stuffs
Model* SwordModel;
Model* StatueModel;
Camera CameraObj;
Skybox* ActiveSkybox;
LightingManager* ActiveLightingManager;

// SkyBox
std::string CubeMapImages[] = { "Resources/Textures/Cubemap/px.png", "Resources/Textures/Cubemap/nx.png",
								"Resources/Textures/Cubemap/py.png", "Resources/Textures/Cubemap/ny.png", 
								"Resources/Textures/Cubemap/pz.png", "Resources/Textures/Cubemap/nz.png" };


// Init Program
GLuint Program_3DShader;
GLuint Program_LightSpheres;



//++++++++++++++++++++++++++


void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	CameraObj.FOV -= yoffset * 3.0f; // Adjust the scaling factor as needed

	if (CameraObj.FOV > CameraObj.MaxFOV)
	{
		CameraObj.FOV = CameraObj.MaxFOV;
	}
	if (CameraObj.FOV < CameraObj.MinFOV)
	{
		CameraObj.FOV = CameraObj.MinFOV;
	}

	std::cout << "FOV: " << CameraObj.FOV << std::endl;
}

void InitialSetup()
{
	// Create Skybox
	ActiveSkybox = new Skybox(&CameraObj, CubeMapImages);

	ActiveLightingManager = new LightingManager(&CameraObj);


	//									Create Model Classes
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	StatueModel = new Model("Resources/Models/SM_Prop_Statue_01.obj", &CameraObj, ActiveSkybox, 1,
							"Resources/Textures/PolygonAncientWorlds_Statue_01.png",
							0.0f, 0.01f, -0.75f, false);

	


	SwordModel = new Model(	"Resources/Models/Sword.obj", &CameraObj, ActiveSkybox, 1000,
							"Resources/Textures/SwordTex.jpg",
							90.0f, 0.02f, 0.0f, true);
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


	// 3D Program Shader
	Program_3DShader = ShaderLoader::CreateProgram(	"Resources/Shaders/Texture.vert",
													"Resources/Shaders/Lighting_BlinnPhong.frag");

	Program_LightSpheres = ShaderLoader::CreateProgram(	"Resources/Shaders/LightSphere.vert",
														"Resources/Shaders/LightSphere.frag");
	// Back Face Culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	// MSAA
	glfwWindowHint(GLFW_SAMPLES, 4);
	glEnable(GL_MULTISAMPLE);

	// Cursor Settings
	glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glfwRawMouseMotionSupported())
	{
		glfwSetInputMode(Window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}

	// Set the colour of the window for when the buffer is cleared
	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);

	// Maps the range of the window size to NDC (-1 -> 1)
	glViewport(0, 0, 800, 800);



}

void Update()
{
	glfwPollEvents();


	if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glDeleteProgram(Program_3DShader);
		glfwDestroyWindow(Window);
		glfwTerminate();
		exit(EXIT_SUCCESS);
	}
	
	if (glfwGetKey(Window, GLFW_KEY_M) == GLFW_PRESS && MeshPreviousKeyState == GLFW_RELEASE)
	{
		// Toggle the state
		MeshToggleState = !MeshToggleState;

		// Output the toggle state for debugging
		if (MeshToggleState)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}
	MeshPreviousKeyState = glfwGetKey(Window, GLFW_KEY_M);


	glfwSetScrollCallback(Window, ScrollCallback);


	// Get Current Time
	PreviousTime = CurrentTime;
	CurrentTime = glfwGetTime();
	DeltaTime = CurrentTime - PreviousTime;


	// Update Objects/Models
	CameraObj.Update(DeltaTime, CurrentTime, Window);
	ActiveLightingManager->Update(Window);
	SwordModel->Update(DeltaTime, Window);
	StatueModel->Update(DeltaTime, Window);

	
}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render Skybox
	ActiveSkybox->Render();

	// Render Lighting
	ActiveLightingManager->Render(&Program_3DShader, &Program_LightSpheres);

	// Render Models
	SwordModel->Render(&Program_3DShader);
	StatueModel->Render(&Program_3DShader);

	// Unbind Assets
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
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
	Window = glfwCreateWindow(WindowWidth, WindowHeight, "Assessment 3", NULL, NULL);
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


