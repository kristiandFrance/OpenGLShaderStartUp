// Library Inlcudes
#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include "ShaderLoader.h"


GLFWwindow* Window = nullptr;	// Window Var.

int Program_FixedTri;


void InitialSetup()
{
	// Set the colour of the window for when the buffer is cleared
	glClearColor(0.1f, 0.05f, 0.2f, 1.0f);

	// Maps the range of the window size to NDC (-1 -> 1)
	glViewport(0, 0, 800, 800);
}

void Update()
{

}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

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
	Window = glfwCreateWindow(800, 800, "YOOOO THIS SHIT WORKS!", NULL, NULL);
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