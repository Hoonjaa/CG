#pragma once
#include "Object.h"
class Square_horn : public Object
{
private:
	GLfloat vertices[5 * 6] = {
		vPos.x + 0.5f, vPos.y - 0.5f, vPos.z + 0.5f,  vColor.r, vColor.g, vColor.b,
		vPos.x - 0.5f, vPos.y - 0.5f, vPos.z + 0.5f,  vColor.r, vColor.g, vColor.b,
		vPos.x - 0.5f, vPos.y - 0.5f, vPos.z - 0.5f,  vColor.r, vColor.g, vColor.b,
		vPos.x + 0.5f, vPos.y - 0.5f, vPos.z - 0.5f,  vColor.r, vColor.g, vColor.b,
		vPos.x,        vPos.y + 0.5f, vPos.z,       vColor.r, vColor.g, vColor.b
	};
	GLuint indexes[6 * 3] = {
		0,2,1,  0,2,3,
		4,2,3,
		4,1,2,
		4,1,0,
		4,3,0
	};
public:
	GLint focus_face[4] = {0,1,2,3};
public:
	Square_horn();
	virtual ~Square_horn();

	GLvoid update_vertex();

	GLvoid draw() override;
	GLvoid update() override;
};

