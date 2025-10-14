#pragma once
#include "Object.h"
class TPentagon13 : public Object
{
private:
	vec3 setVertex[5];
	vec3 ExtraVertex[4];
	GLint ShapeType = 5; // 1:점, 2:선, 3:삼각형, 4:사각형, 5:오각형
	GLfloat size = 1.0f;
	GLfloat speed;
	vec3 dir;
public:
	bool dragging = false;
	bool moving = false;

public:
	TPentagon13(GLfloat x, GLfloat y, GLfloat size, GLint type, GLint mode);
	~TPentagon13();

	GLvoid setDir(vec3 _dir) {
		if (_dir.Length() == 0) _dir = vec3(0.001f, 0.001f, 0.0f);

		_dir.Normalize();
		dir = _dir;

		dir.x *= speed;
		dir.y *= speed;
	}

	vec3 getPos() { return vPos; }
	GLvoid setPos(vec3 _vPos) { vPos = _vPos; }

	GLvoid setDragVertex(GLfloat x, GLfloat y);

	GLint getShapeType() const { return ShapeType; }

	virtual GLvoid draw() override;
	virtual GLvoid update() override;
};

