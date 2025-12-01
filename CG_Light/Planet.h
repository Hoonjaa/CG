#pragma once
#include "Object.h"



class Planet : public Object
{
private:
	std::vector <VertexInfo> vertices;
	std::vector <IndexInfo> indexes;
	GLfloat radius;
	GLfloat orbit_radius = 0.0f;  // 추가: 궤도 반지름 저장

public:
GLfloat revolve_angle = 0.0f;
GLfloat revolve_speed = 0.0f;

bool solid_mode = true;
public:
	Planet(GLfloat r = 1.0f, GLuint sectors = 36, GLuint stacks = 18, glm::vec3 color = { 0.0f, 0.0f, 0.0f }, glm::vec3 Pos = { 0.0f, 0.0f, 0.0f });
	virtual ~Planet();

	GLvoid draw(const GLuint& ShaderID, const glm::mat4& worldMatrix, const glm::mat4& viewProjMatrix) override;
	GLvoid update() override;

	GLvoid setPlanetVertex(GLfloat r = 1.0f, GLuint sectors = 36, GLuint stacks = 18, glm::vec3 color = { 0.0f, 0.0f, 0.0f }, glm::vec3 Pos = { 0.0f, 0.0f, 0.0f });

	GLvoid first_planet_update();
	GLvoid second_planet_update();
	GLvoid third_planet_update();
	
	// 추가: 궤도 반지름 설정 및 위치 업데이트
	GLvoid setOrbitRadius(GLfloat orbit_r);
	GLvoid updatePositionByOrbit();
};