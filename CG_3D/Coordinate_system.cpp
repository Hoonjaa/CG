#include "pch.h"
#include "Coordinate_system.h"

Coordinate_system::Coordinate_system()
{
	GLfloat vertices[] = {
		 -1.0f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
		 1.0f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,

		 0.0f,  -1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f,

		 0.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
		 0.0f,  0.0f, 1.0f,  0.0f, 0.0f, 1.0f
	};
	allocate(sizeof(vertices), GL_STATIC_DRAW);
	upload(vertices, sizeof(vertices), 0);

	setVertexAttrib(0, 3, GL_FLOAT, 6 * sizeof(GLfloat), (GLvoid*)0, GL_FALSE);
	setVertexAttrib(1, 3, GL_FLOAT, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)), GL_FALSE);
	setDrawMode((GLint)DRAWMODE::LINE);
}

Coordinate_system::~Coordinate_system()
{
}

GLvoid Coordinate_system::draw()
{
	glBindVertexArray(VAO);
	glDrawArrays(DrawMode, 0, 6);
}

GLvoid Coordinate_system::update()
{
	return GLvoid();
}
