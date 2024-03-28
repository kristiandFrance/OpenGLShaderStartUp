#include "Quad.h"

Quad::Quad(float _QuadSideLength)
{
	// Populating Variables
	QuadSideLength = _QuadSideLength;
	StartingPoint.x = -QuadSideLength / 2;
	StartingPoint.y = QuadSideLength / 2;


	// Top Left
	Vertices[0].x = StartingPoint.x;
	Vertices[0].y = StartingPoint.y;
	// Bottom Left
	Vertices[1].x = StartingPoint.x;
	Vertices[1].y = StartingPoint.y - QuadSideLength;
	// Bottom Right
	Vertices[2].x = StartingPoint.x + QuadSideLength;
	Vertices[2].y = StartingPoint.y - QuadSideLength;
	// Top Right
	Vertices[3].x = StartingPoint.x + QuadSideLength;
	Vertices[3].y = StartingPoint.y;

	// Setting all Z values to 0 because of 2D
	for (int i = 0; i < 4; i++)
	{
		Vertices[i].z = 0.0f;
	}




	GLfloat _Vertices_Indexed_Quad[] = {
		// Index	// Postition			// Colour			// Texture Coords	
		/* 0 */		Vertices[0].x, Vertices[0].y, Vertices[0].z,	1.0f, 0.0f, 0.0f,	 -2 / 52.0f,  2.0f,			// Top Left
		/* 1 */		Vertices[1].x, Vertices[1].y, Vertices[1].z,	0.0f, 1.0f, 0.0f,	 -2 / 52.0f, -2.0f,			// Bottom Left
		/* 2 */		Vertices[2].x, Vertices[2].y, Vertices[2].z,	1.0f, 0.0f, 1.0f,	  2 / 52.0f, -2.0f,			// Bottom Right
		/* 3 */		Vertices[3].x, Vertices[3].y, Vertices[3].z,	0.0f, 1.0f, 1.0f,	  2 / 52.0f,  2.0f,			// Top Right
	};
	memcpy(&QuadVertices, &_Vertices_Indexed_Quad, 32 * sizeof(GLfloat));

	GLuint _Indices_Quad[] = {
	0, 1, 2,	// First Tri   ( TL -> BL -> BR )
	0, 2, 3,	// Second Tri  ( TL -> BR -> TR )
	};
	memcpy(&QuadIndex, &_Indices_Quad, 6 * sizeof(GLfloat));


}

void Quad::GenerateInfo()
{
	// Generate VAO For Quad
	glGenVertexArrays(1, &getVAO());
	glBindVertexArray(getVAO());

	// Generate EBO For Quad
	glGenBuffers(1, &getEBO());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, getEBO());
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(QuadIndex), QuadIndex, GL_STATIC_DRAW);

	// Generate VBO For Quad
	glGenBuffers(1, &getVBO());
	glBindBuffer(GL_ARRAY_BUFFER, getVBO());
	glBufferData(GL_ARRAY_BUFFER, sizeof(QuadVertices), QuadVertices, GL_STATIC_DRAW);


	// Set the Vertex Attribute Information
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Set the Vertex Color Information
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Set the Texture Information
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
}
