#pragma once
#include "pch.h"
#include "Object.h"

class Triangle : public Object
{
	public:
	Triangle(GLfloat x, GLfloat y);
	~Triangle();

	virtual GLvoid draw() override;
	virtual GLvoid update() override;
};

