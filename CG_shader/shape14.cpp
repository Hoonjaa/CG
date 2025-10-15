#include "pch.h"
#include "shape14.h"

shape14::shape14(GLfloat x, GLfloat y, GLint mode, GLint type)
{
	vPos = vec3(x, y, 0.0f);
	DrawMode = mode;
	vColor = vec3(random_color(rd), random_color(rd), random_color(rd));

	shapeType = type;
	if (shapeType == 1) {
		for (GLint i = 0; i < 12; i++) {
			Vertices[i].x = vPos.x + radius_1[i % 3] * cos(thetaArray_1[i]);
			Vertices[i].y = vPos.y + radius_1[i % 3] * sin(thetaArray_1[i]);
			Vertices[i].z = 0.0f;
		}
	}
	if (shapeType == 2) {
		for (GLint i = 0; i < 12; i++) {
			Vertices[i].x = vPos.x + radius_2[i % 3] * cos(thetaArray_2[i]);
			Vertices[i].y = vPos.y + radius_2[i % 3] * sin(thetaArray_2[i]);
			Vertices[i].z = 0.0f;
		}
	}
	
	GLfloat uploadVertices[12 * 6] = {
		Vertices[0].x, Vertices[0].y, Vertices[0].z,   vColor.x, vColor.y, vColor.z,
		Vertices[1].x, Vertices[1].y, Vertices[1].z,   vColor.x, vColor.y, vColor.z,
		Vertices[2].x, Vertices[2].y, Vertices[2].z,   vColor.x, vColor.y, vColor.z,
		Vertices[3].x, Vertices[3].y, Vertices[3].z,   vColor.x, vColor.y, vColor.z,
		Vertices[4].x, Vertices[4].y, Vertices[4].z,   vColor.x, vColor.y, vColor.z,
		Vertices[5].x, Vertices[5].y, Vertices[5].z,   vColor.x, vColor.y, vColor.z,
		Vertices[6].x, Vertices[6].y, Vertices[6].z,   vColor.x, vColor.y, vColor.z,
		Vertices[7].x, Vertices[7].y, Vertices[7].z,   vColor.x, vColor.y, vColor.z,
		Vertices[8].x, Vertices[8].y, Vertices[8].z,   vColor.x, vColor.y, vColor.z,
		Vertices[9].x, Vertices[9].y, Vertices[9].z,   vColor.x, vColor.y, vColor.z,
		Vertices[10].x, Vertices[10].y, Vertices[10].z,   vColor.x, vColor.y, vColor.z,
		Vertices[11].x, Vertices[11].y, Vertices[11].z,   vColor.x, vColor.y, vColor.z
	};
	
	allocate(sizeof(uploadVertices), GL_STATIC_DRAW);
	upload(uploadVertices, sizeof(uploadVertices));

	GLsizei stride = 6 * sizeof(GLfloat);
	setVertexAttrib(0, 3, GL_FLOAT, stride, (GLvoid*)0);
	setVertexAttrib(1, 3, GL_FLOAT, stride, (GLvoid*)(3 * sizeof(GLfloat)));
	vertexCount = 12;
}

shape14::~shape14()
{
}

GLvoid shape14::draw()
{
	glBindVertexArray(VAO);
	glDrawArrays(DrawMode, 0, vertexCount);
}

GLvoid shape14::update()
{
	return GLvoid();
}
