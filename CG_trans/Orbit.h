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

	GLvoid draw(const GLuint& ShaderID, const glm::mat4& main_matirx) override;
	GLvoid update() override;

	GLvoid setOrbitVertex();
};

