#include "pch.h"
#include "Cube.h"

Cube::Cube()
{
	vColor = glm::vec3(1.0f, 0.5f, 0.0f);
	setVertexInfo();
	// VBO: 정점 데이터
	allocate(sizeof(vertices), GL_STATIC_DRAW);
	upload(vertices, sizeof(vertices));
	// 위치(0), 색상(1), 법선(2)
	setVertexAttrib(0, 3, GL_FLOAT, 9 * sizeof(GLfloat), (void*)0);
	setVertexAttrib(1, 3, GL_FLOAT, 9 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	setVertexAttrib(2, 3, GL_FLOAT, 9 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	// EBO: 인덱스 데이터
	allocateIndex(sizeof(indexes), GL_STATIC_DRAW);
	uploadIndex(indexes, sizeof(indexes));
	vertexCount = 36;
	setDrawMode(GL_TRIANGLES);
}

Cube::~Cube()
{
}

GLvoid Cube::draw(const GLuint& ShaderID, const glm::mat4& main_matirx)
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(VAO);
	glm::mat4 final_matrix = main_matirx * modelMatrix;
	unsigned int modelLocation = glGetUniformLocation(ShaderID, "Transform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(final_matrix));
	glDrawElements(DrawMode, vertexCount, GL_UNSIGNED_INT, 0);
}

GLvoid Cube::update()
{
	return GLvoid();
}
