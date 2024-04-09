#include "Hexagon.h"

Hexagon::Hexagon(float _HexSideLength)
{
	HexSideLength = _HexSideLength;
	StartingPoint.x = -HexSideLength / 2;
	StartingPoint.y = HexSideLength * sin(120 * DegToRad) / sin(30 * DegToRad) / 2;

	// Top Left
	Vertices[0].x = StartingPoint.x;
	Vertices[0].y = StartingPoint.y;
	// Middle Left
	Vertices[1].x = StartingPoint.x - std::sqrt( std::pow(HexSideLength, 2) - std::pow(HexSideLength * (sin(120 * DegToRad) / sin(30 * DegToRad)) / 2, 2) );
	Vertices[1].y = StartingPoint.y - ( HexSideLength * (std::sin(120 * DegToRad) / sin(30 * DegToRad)) ) / 2;
	// Bottom Left
	Vertices[2].x = StartingPoint.x;
	Vertices[2].y = StartingPoint.y - ( HexSideLength * (sin(120 * DegToRad) / sin(30 * DegToRad)) );
	// Bottom Right
	Vertices[3].x = StartingPoint.x + HexSideLength;
	Vertices[3].y = StartingPoint.y - ( HexSideLength * (sin(120 * DegToRad) / sin(30 * DegToRad)) );
	// Middle Right
	Vertices[4].x = StartingPoint.x + HexSideLength + std::sqrt( std::pow(HexSideLength, 2) - std::pow(HexSideLength * (sin(120 * DegToRad) / sin(30 * DegToRad)) / 2, 2) );
	Vertices[4].y = StartingPoint.y - ( HexSideLength * (sin(120 * DegToRad) / sin(30 * DegToRad)) ) / 2;
	// Top Right
	Vertices[5].x = StartingPoint.x + HexSideLength;
	Vertices[5].y = StartingPoint.y;
	// Middle
	Vertices[6].x = 0.0f;
	Vertices[6].y = 0.0f;


	// Setting all Z values to 0 because of 2D
	for (int i = 0; i < 7; i++)
	{
		Vertices[i].z = 0.0f;
	}


	GLfloat _HexVertices[56] = {
		// Index	// Postition									// Colour			// Texture Coords	
		/* 0 */		Vertices[0].x, Vertices[0].y, Vertices[0].z,	1.0f, 0.0f, 0.0f,	0.25 / 52.0f, 1.0f,			// Top Left
		/* 1 */		Vertices[1].x, Vertices[1].y, Vertices[1].z,	1.0f, 1.0f, 0.0f,	0.0 / 52.0f, 0.5f,			// Middle Left
		/* 2 */		Vertices[2].x, Vertices[2].y, Vertices[2].z,	0.0f, 1.0f, 0.0f,	0.25 / 52.0f, 0.0f,			// Bottom Left
		/* 3 */		Vertices[3].x, Vertices[3].y, Vertices[3].z,	0.0f, 1.0f, 1.0f,	0.75 / 52.0f, 0.0f,			// Bottom Right
		/* 4 */		Vertices[4].x, Vertices[4].y, Vertices[4].z,	0.0f, 0.0f, 1.0f,	1.0 / 52.0f, 0.5f,			// Middle Right
		/* 5 */		Vertices[5].x, Vertices[5].y, Vertices[5].z,	1.0f, 0.0f, 1.0f,	0.75 / 52.0f, 1.0f,			// Top Right
		/* 6 */		Vertices[6].x, Vertices[6].y, Vertices[6].z,	1.0f, 1.0f, 1.0f,	0.5f / 52.0f, 0.5f,         // Middle Centre
	};
	memcpy(&HexVertices, &_HexVertices, 56 * sizeof(GLfloat));

	GLuint _HexIndex[18] = {
	0, 1, 6,     // First Tri   ( TL -> ML -> M )
	1, 2, 6,     // Second Tri  ( ML -> BL -> M )
	2, 3, 6,     // Third Tri   ( BL -> BR -> M )
	3, 4, 6,     // Fourth Tri  ( BR -> MR -> M )
	4, 5, 6,     // Fifth Tri   ( MR -> TR -> M )
	5, 0, 6,     // Sixth Tri   ( TR -> TL -> M )
	};
	memcpy(&HexIndex, &_HexIndex, 18 * sizeof(GLuint));

	//DebugOutput();
}

void Hexagon::DebugOutput()
{
	std::cout << "Vertex Top Left = ( " <<		Vertices[0].x << ", " << Vertices[0].y << ", " << Vertices[0].z << " )" << std::endl;
	std::cout << "Vertex Middle Left = ( " <<	Vertices[1].x << ", " << Vertices[1].y << ", " << Vertices[1].z << " )" << std::endl;
	std::cout << "Vertex Bottom Left = ( " <<	Vertices[2].x << ", " << Vertices[2].y << ", " << Vertices[2].z << " )" << std::endl;
	std::cout << "Vertex Bottom Right = ( " <<	Vertices[3].x << ", " << Vertices[3].y << ", " << Vertices[3].z << " )" << std::endl;
	std::cout << "Vertex Middle Right = ( " <<	Vertices[4].x << ", " << Vertices[4].y << ", " << Vertices[4].z << " )" << std::endl;
	std::cout << "Vertex Top Right = ( " <<		Vertices[5].x << ", " << Vertices[5].y << ", " << Vertices[5].z << " )" << std::endl;
}

void Hexagon::GenerateInfo()
{

	// Generate VAO For Hex
	glGenVertexArrays(1, &getVAO());
	glBindVertexArray(getVAO());

	// Generate EBO For Hex
	glGenBuffers(1, &getEBO());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, getEBO());
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(HexIndex), HexIndex, GL_STATIC_DRAW);

	// Generate VBO For Hex
	glGenBuffers(1, &getVBO());
	glBindBuffer(GL_ARRAY_BUFFER, getVBO());
	glBufferData(GL_ARRAY_BUFFER, sizeof(HexVertices), HexVertices, GL_STATIC_DRAW);


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



