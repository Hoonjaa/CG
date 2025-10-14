#include "pch.h"
#include "TPentagon.h"

TPentagon::TPentagon(GLfloat x, GLfloat y, GLint mode)
{
	ShapeType = 0;
	vPos = vec3(x, y, 0.0f);
	ExtraVertex[0] = vec3(vPos.x - 0.2f, vPos.y - 0.1f, 0.0f);
	ExtraVertex[1] = vec3(vPos.x - 0.14f, vPos.y - 0.3f, 0.0f);
	ExtraVertex[2] = vec3(vPos.x + 0.14f, vPos.y - 0.3f, 0.0f);
	ExtraVertex[3] = vec3(vPos.x + 0.2f, vPos.y - 0.1f, 0.0f);
	vColor = vec3(random_color(rd), random_color(rd), random_color(rd));
	DrawMode = mode;

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

TPentagon::~TPentagon()
{
}

GLvoid TPentagon::draw()
{
	glBindVertexArray(VAO);
	glDrawArrays(DrawMode, 0, vertexCount);
}

GLvoid TPentagon::update()
{
	if (ShapeType == 0) {
		TransPentagon();
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

GLvoid TPentagon::TransPentagon()
{
	GLint finish = 0;
	//v->3
	if (vPos.x < ExtraVertex[3].x) {
		vPos.x += 0.01f;
	}
	else {
		vPos.x = ExtraVertex[3].x;
		finish++;
	}
	if (vPos.y > ExtraVertex[3].y) {
		vPos.y -= 0.01f;
	}
	else {
		vPos.y = ExtraVertex[3].y;
		finish++;
	}
	//0->1
	if (ExtraVertex[0].x < ExtraVertex[1].x) {
		ExtraVertex[0].x += 0.01f;
	}
	else {
		ExtraVertex[0].x = ExtraVertex[1].x;
		finish++;
	}
	if (ExtraVertex[0].y > ExtraVertex[1].y) {
		ExtraVertex[0].y -= 0.01f;
	}
	else {
		ExtraVertex[0].y = ExtraVertex[1].y;
		finish++;
	}
	//2->1
	if (ExtraVertex[2].x > ExtraVertex[1].x) {
		ExtraVertex[2].x -= 0.01f;
	}
	else {
		ExtraVertex[2].x = ExtraVertex[1].x;
		finish++;
	}

	if (finish == 5) {
		ShapeType = 1;
		DrawMode = (GLint)DRAWMODE::LINE;
	}
}