#pragma once
#include "Shape.h"
#define DegToRad 0.0174532925199432f


class Hexagon : public Shape
{
public:
	Hexagon(float _HexSideLength);
	~Hexagon() {};

	void DebugOutput();

	void GenerateInfo();

	vertex Vertices[7];
	GLfloat HexVertices[56];
	GLuint HexIndex[18];

private:
	vertex StartingPoint;
	float HexSideLength;

};

