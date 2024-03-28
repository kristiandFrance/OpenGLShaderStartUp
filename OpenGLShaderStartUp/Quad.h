#pragma once
#include "Shape.h"


class Quad : public Shape
{
public:
	Quad(float _QuadSideLength);
	~Quad() {};

	void GenerateInfo();

	vertex Vertices[4];
	GLfloat QuadVertices[32];
	GLuint QuadIndex[6];

private:
	vertex StartingPoint;
	float QuadSideLength;
};

