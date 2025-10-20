#pragma once
#include "Object.h"
class shape14 : public Object
{
private:
	GLint shapeType = 1;
	GLfloat pi = 3.14159265358979323846f;
	GLfloat size = 1.0f;

	vec3 Vertices[12];
	GLfloat thetaArray_1[12] = {
		pi * 0.25f, pi * 0.5f, pi * 0.75f
		,pi * 0.75f, pi * 1.0f, pi * 1.25f
		,pi * 1.25f, pi * 1.5f, pi * 1.75f
		,pi * 1.75f, pi * 2.0f, pi * 2.25f
	};
	GLfloat thetaArray_2[12] = {
		pi * 0.25f, pi * 0.75f, pi * 0.5f
		,pi * 0.75f, pi * 1.25f, pi * 1.0f
		,pi * 1.25f, pi * 1.75f, pi * 1.5f
		,pi * 1.75f, pi * 2.25f, pi * 2.0f
	};

	GLfloat radius_1[3] = {
		0.2f * size, 0.28f * size, 0.2f * size
	};
	GLfloat radius_2[3] = {
		0.2f * size, 0.2f * size, 0.02f * size
	};

	GLfloat dTheta = 0.05f;     // 각도 증가량

public:
	shape14(GLfloat x, GLfloat y, GLint mode, GLint type);
	~shape14();

	GLvoid setDir_t() { dTheta = 0.05f; }
	GLvoid setDir_c() { dTheta = -0.05f; }

	vec3 getPos() { return vPos; }
	GLvoid setPos(vec3 pos) { vPos = pos; }

	virtual GLvoid draw() override;
	virtual GLvoid update() override;
};

