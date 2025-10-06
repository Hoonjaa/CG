#include "Axis.h"

static const GLfloat axisVertices[] = {
	// X axis line
	-1.f, 0.f, 0.f,   1.f, 0.f, 0.f,   // vertex 0: pos + red
	 1.f, 0.f, 0.f,   1.f, 0.f, 0.f,   // vertex 1: pos + red
	 // Y axis line
	  0.f,-1.f, 0.f,   1.f, 0.f, 0.f,   // vertex 2: pos + red
	  0.f, 1.f, 0.f,   1.f, 0.f, 0.f    // vertex 3: pos + red
};

Axis::Axis()
{
	allocate(sizeof(axisVertices), GL_STATIC_DRAW);
	upload(axisVertices, sizeof(axisVertices));

	GLsizei stride = 6 * sizeof(GLfloat);
	setVertexAttrib(0, 3, GL_FLOAT, stride, (GLvoid*)0);
	setVertexAttrib(1, 3, GL_FLOAT, stride, (GLvoid*)(3 * sizeof(GLfloat)));
	vertexCount = 4;
}

Axis::~Axis()
{
}

GLvoid Axis::draw()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, vertexCount);
}

GLvoid Axis::update()
{
	
}
