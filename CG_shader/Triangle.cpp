#include "pch.h"
#include "Triangle.h"

std::random_device rd;
std::uniform_real_distribution<float> random_color(0.0f, 1.0f);
std::uniform_real_distribution<float> random_size(0.07f, 0.15f);

Triangle::Triangle(GLfloat x,  GLfloat y, GLint mode)
{
	vec3 vPos = vec3(x, y, 0.0f);
	vec3 vColor = vec3(random_color(rd), random_color(rd), random_color(rd));
	DrawMode = mode;

	GLfloat size = random_size(rd);

	GLfloat triangleVertices[] = {
		// Triangle vertices
		vPos.x - size, vPos.y, vPos.z,			vColor.x, vColor.y, vColor.z,
		vPos.x + size, vPos.y, vPos.z,			vColor.x, vColor.y, vColor.z,
		vPos.x, vPos.y + (3 * size), vPos.z,    vColor.x, vColor.y, vColor.z
	};

	allocate(sizeof(triangleVertices), GL_STATIC_DRAW);
	upload(triangleVertices, sizeof(triangleVertices));

	GLsizei stride = 6 * sizeof(GLfloat);
	setVertexAttrib(0, 3, GL_FLOAT, stride, (GLvoid*)0);
	setVertexAttrib(1, 3, GL_FLOAT, stride, (GLvoid*)(3 * sizeof(GLfloat)));
	vertexCount = 3;
}

Triangle::~Triangle()
{
}

GLvoid Triangle::draw()
{
	glBindVertexArray(VAO);
	glDrawArrays(DrawMode, 0, vertexCount);
}

GLvoid Triangle::update()
{
	
}
