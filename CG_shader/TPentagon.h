#pragma once
#include "Object.h"
class TPentagon : public Object
{
private:
	vec3 ExtraVertex[4];
public:
	TPentagon(GLfloat x, GLfloat y, GLint mode);
	~TPentagon();

	virtual GLvoid draw() override;
	virtual GLvoid update() override;
};

