#pragma once
#include "pch.h"
#include "Object.h"

class Triangle : public Object
{
private:
	GLfloat size;
	GLfloat speed;
	vec3 dir;
public:
	Triangle(GLfloat x, GLfloat y, GLint mode);
	~Triangle();

	GLvoid setDir(vec3 _dir) {
		if (_dir.Length() == 0) _dir = vec3(0.001f, 0.001f, 0.0f);

		_dir.Normalize();
		dir = _dir;

		dir.x *= speed;
		dir.y *= speed;
	}

	GLvoid setMoveType(GLint type) { moveType = type; }

	virtual GLvoid draw() override;
	virtual GLvoid update() override;
};

