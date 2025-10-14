#include "pch.h"
#include "TPentagon13.h"

TPentagon13::TPentagon13(GLfloat x, GLfloat y, GLfloat size, GLint type, GLint mode)
{
	this->size = size;
	DrawMode = mode;
	vPos = vec3(x, y, 0.0f);
	ExtraVertex[0] = vec3(vPos.x - size * 0.2f, vPos.y - size * 0.1f, 0.0f);
	ExtraVertex[1] = vec3(vPos.x - size * 0.14f, vPos.y - size * 0.3f, 0.0f);
	ExtraVertex[2] = vec3(vPos.x + size * 0.14f, vPos.y - size * 0.3f, 0.0f);
	ExtraVertex[3] = vec3(vPos.x + size * 0.2f, vPos.y - size * 0.1f, 0.0f);
	setVertex[0] = ExtraVertex[0];
	setVertex[1] = ExtraVertex[1];
	setVertex[2] = ExtraVertex[2];
	setVertex[3] = ExtraVertex[3];
	setVertex[4] = vPos;

	ShapeType = type;
	if (ShapeType == 1) {
		DrawMode = (GLint)DRAWMODE::LINE;
		vPos = setVertex[3];
		ExtraVertex[0] = setVertex[1];
		ExtraVertex[2] = setVertex[1];
	}
	if (ShapeType == 2) {
		vPos = setVertex[4];
		ExtraVertex[0] = setVertex[1];
		ExtraVertex[2] = setVertex[2];
		ExtraVertex[3] = setVertex[2];
	}
	if (ShapeType == 3) {
		vPos = setVertex[0];
		ExtraVertex[1] = vec3(setVertex[1].x - size * 0.06f, setVertex[1].y, 0.0f);
		ExtraVertex[2] = vec3(setVertex[2].x + size * 0.06f, setVertex[2].y, 0.0f);
	}
	if (ShapeType == 4) {
		DrawMode = (GLint)DRAWMODE::POINT;
		ExtraVertex[0] = setVertex[4];
		ExtraVertex[1] = setVertex[4];
		ExtraVertex[2] = setVertex[4];
		ExtraVertex[3] = setVertex[4];
	}

	vColor = vec3(random_color(rd), random_color(rd), random_color(rd));

	GLfloat pentagonVertices[] = {
		vPos.x, vPos.y, vPos.z,         vColor.x, vColor.y, vColor.z,
		ExtraVertex[0].x, ExtraVertex[0].y, ExtraVertex[0].z,   vColor.x, vColor.y, vColor.z,
		ExtraVertex[3].x, ExtraVertex[3].y, ExtraVertex[3].z,   vColor.x, vColor.y, vColor.z,

		ExtraVertex[3].x, ExtraVertex[3].y, ExtraVertex[3].z,   vColor.x, vColor.y, vColor.z,
		ExtraVertex[0].x, ExtraVertex[0].y, ExtraVertex[0].z,   vColor.x, vColor.y, vColor.z,
		ExtraVertex[1].x, ExtraVertex[1].y, ExtraVertex[1].z,   vColor.x, vColor.y, vColor.z,

		ExtraVertex[3].x, ExtraVertex[3].y, ExtraVertex[3].z,   vColor.x, vColor.y, vColor.z,
		ExtraVertex[1].x, ExtraVertex[1].y, ExtraVertex[1].z,   vColor.x, vColor.y, vColor.z,
		ExtraVertex[2].x, ExtraVertex[2].y, ExtraVertex[2].z,   vColor.x, vColor.y, vColor.z
	};

	allocate(sizeof(pentagonVertices), GL_STATIC_DRAW);
	upload(pentagonVertices, sizeof(pentagonVertices));

	GLsizei stride = 6 * sizeof(GLfloat);
	setVertexAttrib(0, 3, GL_FLOAT, stride, (GLvoid*)0);
	setVertexAttrib(1, 3, GL_FLOAT, stride, (GLvoid*)(3 * sizeof(GLfloat)));
	vertexCount = 9;
}

TPentagon13::~TPentagon13()
{
}

GLvoid TPentagon13::setDragVertex(GLfloat x, GLfloat y)
{
	vPos = vec3(x, y, 0.0f);
	ExtraVertex[0] = vec3(vPos.x - size * 0.2f, vPos.y - size * 0.1f, 0.0f);
	ExtraVertex[1] = vec3(vPos.x - size * 0.14f, vPos.y - size * 0.3f, 0.0f);
	ExtraVertex[2] = vec3(vPos.x + size * 0.14f, vPos.y - size * 0.3f, 0.0f);
	ExtraVertex[3] = vec3(vPos.x + size * 0.2f, vPos.y - size * 0.1f, 0.0f);
	setVertex[0] = ExtraVertex[0];
	setVertex[1] = ExtraVertex[1];
	setVertex[2] = ExtraVertex[2];
	setVertex[3] = ExtraVertex[3];
	setVertex[4] = vPos;

	if (ShapeType == 1) {
		DrawMode = (GLint)DRAWMODE::LINE;
		vPos = setVertex[3];
		ExtraVertex[0] = setVertex[1];
		ExtraVertex[2] = setVertex[1];
		vPos.x -= size * 0.2f;
		vPos.y += size * 0.1f;
		for(int i=0; i<4; i++) {
			ExtraVertex[i].x -= size * 0.2f;
			ExtraVertex[i].y += size * 0.1f;
		}
	}
	if (ShapeType == 2) {
		vPos = setVertex[4];
		ExtraVertex[0] = setVertex[1];
		ExtraVertex[2] = setVertex[2];
		ExtraVertex[3] = setVertex[2];
	}
	if (ShapeType == 3) {
		vPos = setVertex[0];
		ExtraVertex[1] = vec3(setVertex[1].x - size * 0.06f, setVertex[1].y, 0.0f);
		ExtraVertex[2] = vec3(setVertex[2].x + size * 0.06f, setVertex[2].y, 0.0f);
		vPos.x += size * 0.2f;
		vPos.y += size * 0.1f;
		for (int i = 0; i < 4; i++) {
			ExtraVertex[i].x += size * 0.2f;
			ExtraVertex[i].y += size * 0.1f;
		}
	}
	if (ShapeType == 4) {
		DrawMode = (GLint)DRAWMODE::POINT;
		ExtraVertex[0] = setVertex[4];
		ExtraVertex[1] = setVertex[4];
		ExtraVertex[2] = setVertex[4];
		ExtraVertex[3] = setVertex[4];
	}

	GLfloat pentagonVertices[] = {
		vPos.x, vPos.y, vPos.z,         vColor.x, vColor.y, vColor.z,
		ExtraVertex[0].x, ExtraVertex[0].y, ExtraVertex[0].z,   vColor.x, vColor.y, vColor.z,
		ExtraVertex[3].x, ExtraVertex[3].y, ExtraVertex[3].z,   vColor.x, vColor.y, vColor.z,

		ExtraVertex[3].x, ExtraVertex[3].y, ExtraVertex[3].z,   vColor.x, vColor.y, vColor.z,
		ExtraVertex[0].x, ExtraVertex[0].y, ExtraVertex[0].z,   vColor.x, vColor.y, vColor.z,
		ExtraVertex[1].x, ExtraVertex[1].y, ExtraVertex[1].z,   vColor.x, vColor.y, vColor.z,

		ExtraVertex[3].x, ExtraVertex[3].y, ExtraVertex[3].z,   vColor.x, vColor.y, vColor.z,
		ExtraVertex[1].x, ExtraVertex[1].y, ExtraVertex[1].z,   vColor.x, vColor.y, vColor.z,
		ExtraVertex[2].x, ExtraVertex[2].y, ExtraVertex[2].z,   vColor.x, vColor.y, vColor.z
	};
	upload(pentagonVertices, sizeof(pentagonVertices));
}

GLvoid TPentagon13::draw()
{
	glBindVertexArray(VAO);
	glDrawArrays(DrawMode, 0, vertexCount);
}

GLvoid TPentagon13::update()
{
	
}