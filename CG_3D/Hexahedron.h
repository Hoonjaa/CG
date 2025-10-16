#pragma once
#include "Object.h"
class Hexahedron : public Object
{
private:
	GLfloat vertices[4 * 6] = {
		vPos.x + 0.5f, vPos.y + 0.5f, vPos.z + 0.5f,  vColor.r, vColor.g, vColor.b,
		vPos.x - 0.5f, vPos.y + 0.5f, vPos.z + 0.5f,  vColor.r, vColor.g, vColor.b,
		vPos.x - 0.5f, vPos.y - 0.5f, vPos.z + 0.5f,  vColor.r, vColor.g, vColor.b,
		vPos.x + 0.5f, vPos.y - 0.5f, vPos.z + 0.5f,  vColor.r, vColor.g, vColor.b,
	};
	GLuint indexes[6] = {
		0,1,2,  0,2,3
	};
public:
	Hexahedron();
	virtual ~Hexahedron();

	GLvoid update_vertex();

	GLvoid draw() override;
	GLvoid update() override;
};

