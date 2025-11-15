#include "pch.h"
#include "Obstacle.h"

Obstacle::Obstacle()
{
	vPos = glm::vec3(random_obstacle(rd), -4.6f, random_obstacle(rd));
	vColor = glm::vec3(0.6f, 0.3f, 0.3f);
	setVertexInfo();

	// VBO: 정점 데이터
	allocate(sizeof(vertices), GL_STATIC_DRAW);
	upload(vertices, sizeof(vertices));

	// 위치(0), 색상(1)
	setVertexAttrib(0, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void*)0);
	setVertexAttrib(1, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	// EBO: 인덱스 데이터
	allocateIndex(sizeof(indexes), GL_STATIC_DRAW);
	uploadIndex(indexes, sizeof(indexes));

	setDrawMode(GL_TRIANGLES);
}

Obstacle::~Obstacle()
{
}

GLvoid Obstacle::draw(const GLuint& ShaderID, const glm::mat4& main_matirx)
{
	glUseProgram(ShaderID);

	// Transform 행렬 전달
	GLuint transformLoc = glGetUniformLocation(ShaderID, "Transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(main_matirx));

	glBindVertexArray(VAO);
	glDrawElements(DrawMode, 36, GL_UNSIGNED_INT, 0);
}

GLvoid Obstacle::update()
{
	return GLvoid();
}
