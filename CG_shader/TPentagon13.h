#pragma once
#include "Object.h"
class TPentagon13 : public Object
{
private:
	vec3 setVertex[5];
	vec3 ExtraVertex[4];
	GLint ShapeType = 0; //0:오각형, 1:선, 2:삼각형, 3:사각형, 4:점
	GLfloat size = 1.0f;
public:
	TPentagon13(GLfloat x, GLfloat y, GLfloat size, GLint type, GLint mode);
	~TPentagon13();

	virtual GLvoid draw() override;
	virtual GLvoid update() override;
};

