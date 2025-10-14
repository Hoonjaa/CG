#include "pch.h"
#include "TPentagon.h"

TPentagon::TPentagon(GLfloat x, GLfloat y, GLfloat size, GLint mode)
{
	ShapeType = 0;
	this->size = size;
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

	if (ShapeType == 1) {
		TransLine();
	}

	if (ShapeType == 2) {
		TransTriangle();
	}

	if (ShapeType == 3) {
		TransRectangle();
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

GLvoid TPentagon::TransLine()
{
	if (DrawMode != (GLint)DRAWMODE::TRIANGLE)
		DrawMode = (GLint)DRAWMODE::TRIANGLE;
	GLint finish = 0;

	//v 이동
	if (vPos.x > setVertex[4].x) {
		vPos.x -= 0.01f;
	}
	else {
		vPos.x = setVertex[4].x;
		finish++;
	}
	if (vPos.y < setVertex[4].y) {
		vPos.y += 0.01f;
	}
	else {
		vPos.y = setVertex[4].y;
		finish++;
	}
	//3 이동
	if (ExtraVertex[3].x > setVertex[2].x) {
		ExtraVertex[3].x -= 0.01f;
	}
	else {
		ExtraVertex[3].x = setVertex[2].x;
		finish++;
	}
	if (ExtraVertex[3].y > setVertex[2].y) {
		ExtraVertex[3].y -= 0.01f;
	}
	else {
		ExtraVertex[3].y = setVertex[2].y;
		finish++;
	}

	if (finish == 4) {
		ShapeType = 2;
	}
}

GLvoid TPentagon::TransTriangle()
{
	GLint finish = 0;
	
	//v 이동
	if (vPos.x > setVertex[0].x) {
		vPos.x -= 0.01f;
	}
	else {
		vPos.x = setVertex[0].x;
		finish++;
	}
	if (vPos.y > setVertex[0].y) {
		vPos.y -= 0.01f;
	}
	else {
		vPos.y = setVertex[0].y;
		finish++;
	}
	//0 이동
	if (ExtraVertex[0].x > setVertex[0].x) {
		ExtraVertex[0].x -= 0.01f;
	}
	else {
		ExtraVertex[0].x = setVertex[0].x;
		finish++;
	}
	if (ExtraVertex[0].y < setVertex[0].y) {
		ExtraVertex[0].y += 0.01f;
	}
	else {
		ExtraVertex[0].y = setVertex[0].y;
		finish++;
	}
	//1 이동
	if (ExtraVertex[1].x > setVertex[1].x - size * 0.06f) {
		ExtraVertex[1].x -= 0.01f;
	}
	else {
		ExtraVertex[1].x = setVertex[1].x - size * 0.06f;
		finish++;
	}
	//2 이동
	if (ExtraVertex[2].x < setVertex[2].x + size * 0.06f) {
		ExtraVertex[2].x += 0.01f;
	}
	else {
		ExtraVertex[2].x = setVertex[2].x + size * 0.06f;
		finish++;
	}
	//3 이동
	if (ExtraVertex[3].x < setVertex[3].x) {
		ExtraVertex[3].x += 0.01f;
	}
	else {
		ExtraVertex[3].x = setVertex[3].x;
		finish++;
	}
	if (ExtraVertex[3].y < setVertex[3].y) {
		ExtraVertex[3].y += 0.01f;
	}
	else {
		ExtraVertex[3].y = setVertex[3].y;
		finish++;
	}

	if(finish == 8) {
		ShapeType = 3;
	}
}

GLvoid TPentagon::TransRectangle()
{
	GLint finish = 0;
	//v 이동
	if (vPos.x < setVertex[4].x) {
		vPos.x += 0.01f;
	}
	else {
		vPos.x = setVertex[4].x;
		finish++;
	}
	if (vPos.y < setVertex[4].y) {
		vPos.y += 0.01f;
	}
	else {
		vPos.y = setVertex[4].y;
		finish++;
	}
	//1 이동
	if (ExtraVertex[1].x < setVertex[1].x) {
		ExtraVertex[1].x += 0.01f;
	}
	else {
		ExtraVertex[1].x = setVertex[1].x;
		finish++;
	}
	//2 이동
	if (ExtraVertex[2].x > setVertex[2].x) {
		ExtraVertex[2].x -= 0.01f;
	}
	else {
		ExtraVertex[2].x = setVertex[2].x;
		finish++;
	}

	if (finish == 4) {
		ShapeType = 0;
	}
}