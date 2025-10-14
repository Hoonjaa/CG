#pragma once
#include "Object.h"
class TPentagon : public Object
{
private:
	vec3 ExtraVertex[4];
	GLint ShapeType = 0; //0:오각형, 1:선, 2:삼각형, 3:사각형

	GLvoid TransPentagon();
public:
	TPentagon(GLfloat x, GLfloat y, GLint mode);
	~TPentagon();

	virtual GLvoid draw() override;
	virtual GLvoid update() override;
};

