// Library Inlcudes
#include <glew.h>
#include <glfw3.h>
#include <iostream>
#include "ShaderLoader.h"


//++++++++++++++++++++++++++
//		GLOBAL VARS

GLFWwindow* Window = nullptr;	// Window Var.
int Program_FixedTri;			// Program Var.
float CurrentTime;

// Tri. Verts
GLfloat Vertices_Tri[] = {
	// Postition		// Colour
	 0.0f, 0.0f, 0.0f,	1.0f, 0.0f, 0.0f,	 // Top Right
	-0.5f, 0.8f, 0.0f,	0.0f, 1.0f, 0.0f,	 // Top Left
	 0.5f, 0.8f, 0.0f,	0.0f, 0.0f, 1.0f,	 // Bottom Center
};

// Quad Tri Verts
GLfloat Vertices_Quad[] = {
	// Postition			// Colour
	-0.3f,  0.3f,  0.0f,	1.0f, 0.2f, 0.0f,	 // Top Left
	-0.3f, -0.3f,  0.0f,	0.0f, 1.0f, 0.5f,	 // Bottom Left
	 0.3f, -0.3f,  0.0f,	0.6f, 1.0f, 0.0f,	 // Bottom Right
	-0.3f,  0.3f,  0.0f,	1.0f, 0.2f, 0.0f,	 // Top Left
	 0.3f,  0.3f,  0.0f,	0.4f, 0.0f, 1.0f,	 // Top Right
	 0.3f, -0.3f,  0.0f,	0.6f, 1.0f, 0.0f,	 // Bottom Right

};

// Quad2 Tri Verts
GLfloat Vertices_Quad2[] = {
	// Postition			// Colour
	-0.1f,  0.1f,  0.0f,	1.0f, 0.2f, 0.0f,	 // Top Left
	-0.1f, -0.1f,  0.0f,	0.0f, 1.0f, 0.5f,	 // Bottom Left
	 0.1f, -0.1f,  0.0f,	0.6f, 1.0f, 0.0f,	 // Bottom Right
	-0.1f,  0.1f,  0.0f,	1.0f, 0.2f, 0.0f,	 // Top Left
	 0.1f,  0.1f,  0.0f,	0.4f, 0.0f, 1.0f,	 // Top Right
	 0.1f, -0.1f,  0.0f,	0.6f, 1.0f, 0.0f,	 // Bottom Right

};

GLuint Program_ColorFade;

// Creating Tri. VBO/VAO
//GLuint VBO_Tri;
//GLuint VAO_Tri;

// Creating Quad VBO/VAO
GLuint VBO_Quad;
GLuint VAO_Quad;
GLuint VBO_Quad2;
GLuint VAO_Quad2;


//++++++++++++++++++++++++++




void InitialSetup()
{



	Program_ColorFade = ShaderLoader::CreateProgram( "Resources/Shaders/VertexColor.vert",
													 "Resources/Shaders/VertexColorChange.frag");


	// Generate The VAO For Tri.
	/*glGenVertexArrays(1, &VAO_Tri);
	glBindVertexArray(VAO_Tri);*/

	// Generate The VAO For Quad
	glGenVertexArrays(1, &VAO_Quad);
	glBindVertexArray(VAO_Quad);

	//glGenVertexArrays(1, &VAO_Quad2);
	//glBindVertexArray(VAO_Quad2);

	// Generate The VBO For Tri.
	/*glGenBuffers(1, &VBO_Tri);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Tri);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices_Tri), Vertices_Tri, GL_STATIC_DRAW);*/

	glGenBuffers(1, &VBO_Quad);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Quad);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices_Quad), Vertices_Quad, GL_STATIC_DRAW);

	//glGenBuffers(1, &VBO_Quad2);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO_Quad2);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices_Quad2), Vertices_Quad2, GL_STATIC_DRAW);


	// Set the Vertex Attribute Information
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Set the Vertex Color Information
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);


	// Set the colour of the window for when the buffer is cleared
	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);

	// Maps the range of the window size to NDC (-1 -> 1)
	glViewport(0, 0, 800, 800);
}

void Update()
{
	glfwPollEvents();

	// Get Current Time
	CurrentTime = glfwGetTime();
}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	// Bind Assets
	glUseProgram(Program_ColorFade);
	glBindVertexArray(VAO_Quad);
	//glBindVertexArray(VAO_Quad2);

	// Send Vars to shaders via "Uniform"
	GLint CurrentTimeLoc = glGetUniformLocation(Program_ColorFade, "CurrentTime");
	glUniform1f(CurrentTimeLoc, CurrentTime);

	// Render the Tri.
	glDrawArrays(GL_TRIANGLES, 0, 6);


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