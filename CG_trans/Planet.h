#pragma once
#include "Object.h"

struct VertexInfo
{
	glm::vec3 vPos;
	glm::vec3 vColor;
};

struct IndexInfo
{
	GLuint index[3];
};

class Planet : public Object
{
private:
	std::vector <VertexInfo> vertices;
	std::vector <IndexInfo> indexes;
	GLfloat radius;

public:
	Planet(GLfloat r = 1.0f, GLuint sectors = 36, GLuint stacks = 18, glm::vec3 color = { 0.0f, 0.0f, 0.0f }, glm::vec3 Pos = { 0.0f, 0.0f, 0.0f });
	virtual ~Planet();

	GLvoid draw(const GLuint& ShaderID, const glm::mat4& main_matirx) override;
	GLvoid update() override;

	GLvoid setPlanetVertex(GLfloat r = 1.0f, GLuint sectors = 36, GLuint stacks = 18, glm::vec3 color = { 0.0f, 0.0f, 0.0f }, glm::vec3 Pos = { 0.0f, 0.0f, 0.0f });

	GLvoid first_planet_update();
	GLvoid second_planet_update();
	GLvoid third_planet_update();
};