#pragma once
#include "Object.h"
class Hexahedron : public Object
{
private:
	GLfloat vertices[6 * 8] = {
		vPos.x + 0.5f, vPos.y + 0.5f, vPos.z + 0.5f,  vColor.r, vColor.g, vColor.b,
		vPos.x - 0.5f, vPos.y + 0.5f, vPos.z + 0.5f,  vColor.r, vColor.g, vColor.b,
		vPos.x - 0.5f, vPos.y - 0.5f, vPos.z + 0.5f,  vColor.r, vColor.g, vColor.b,
		vPos.x + 0.5f, vPos.y - 0.5f, vPos.z + 0.5f,  vColor.r, vColor.g, vColor.b,
		vPos.x - 0.5f, vPos.y + 0.5f, vPos.z - 0.5f,  vColor.r, vColor.g, vColor.b,
		vPos.x - 0.5f, vPos.y - 0.5f, vPos.z - 0.5f,  vColor.r, vColor.g, vColor.b,
		vPos.x + 0.5f, vPos.y + 0.5f, vPos.z - 0.5f,  vColor.r, vColor.g, vColor.b,
		vPos.x + 0.5f, vPos.y - 0.5f, vPos.z - 0.5f,  vColor.r, vColor.g, vColor.b,
	};
	GLuint indexes[6 * 6] = {
		0,1,2,  0,2,3,	// ¾Õ¸é
		1,4,5,  1,5,2,	// ¿ÞÂÊ¸é
		5,4,6,  5,6,7,	// µÞ¸é
		7,0,3,  7,6,0,	// ¿À¸¥ÂÊ¸é
		2,5,7,  2,7,3,	// ¾Æ·¡¸é
		1,0,6,  1,6,4	// À­¸é
	};
public:
	GLint face = -1;
public:
	Hexahedron();
	virtual ~Hexahedron();

	GLvoid update_vertex();

	GLvoid draw() override;
	GLvoid update() override;
};

