#pragma once
#include "Shape.h"


class Quad : public Shape
{
public:
	Quad(float _QuadSideLength, bool _IsAnimated);
	~Quad() {};

	void GenerateInfo();
	virtual void Update(float DeltaTime, GLFWwindow* _WindowRef);
	virtual void Render(GLuint* Program);

	vertex Vertices[4];
	GLfloat QuadVertices[32];
	GLuint QuadIndex[6];

private:
	vertex StartingPoint;
	float QuadSideLength;
	bool IsAnimated;
};

