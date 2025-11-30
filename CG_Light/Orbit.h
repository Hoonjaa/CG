#pragma once
#include "Object.h"

class Orbit : public Object
{
private:
	std::vector <VertexInfo> vertices;

public:
	GLfloat radius;

public:
	Orbit(GLfloat r = 0.0f);
	virtual ~Orbit();

	GLvoid draw(const GLuint& ShaderID, const glm::mat4& worldMatrix, const glm::mat4& viewProjMatrix) override;
	GLvoid update() override;

	GLvoid setOrbitVertex();
	
	// 궤도 상의 위치를 반환하는 함수
	glm::vec3 getPositionOnOrbit(GLfloat angle) const;
};

