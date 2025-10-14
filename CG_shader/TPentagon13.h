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
	bool dragging = false;
	bool moving = false;

public:
	TPentagon13(GLfloat x, GLfloat y, GLfloat size, GLint type, GLint mode);
	~TPentagon13();

	vec3 getPos() { return vPos; }
	GLvoid setPos(vec3 _vPos) { vPos = _vPos; }

	GLvoid setDragVertex(GLfloat x, GLfloat y);

	virtual GLvoid draw() override;
	virtual GLvoid update() override;
};

