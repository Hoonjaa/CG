#pragma once
#include "Object.h"
class Spiral : public Object
{
private:
	// 동적 증가용
	std::vector<vec3> vertices;
	GLint capacity = 0;

	// 스파이럴 파라미터
	GLfloat radius = 0.0f;
	GLfloat theta = 0.0f;
	GLfloat dTheta = 0.15f;     // 각도 증가량
	GLfloat k = 0.01f;          // r = k * theta (아르키메데스 나선)
	GLfloat phase = 0.0f;      // 위치 보정용

	// 필요 시 버퍼(capacity) 확장
	GLvoid ensureCapacity(GLint needed);
public:
	Spiral(GLfloat x, GLfloat y, GLint mode);
	~Spiral();

	virtual GLvoid draw() override;
	virtual GLvoid update() override;
};

