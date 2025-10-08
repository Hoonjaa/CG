#include "pch.h"
#include "Triangle.h"

std::random_device rd;
std::uniform_real_distribution<float> random_color(0.0f, 1.0f);
std::uniform_real_distribution<float> random_size(0.07f, 0.15f);
std::uniform_real_distribution<float> random_dir(-0.01f, 0.01f);
std::uniform_real_distribution<float> random_speed(0.005f, 0.02f);

Triangle::Triangle(GLfloat x,  GLfloat y, GLint mode)
{
	vPos = vec3(x, y, 0.0f);
	vColor = vec3(random_color(rd), random_color(rd), random_color(rd));
	DrawMode = mode;
	speed = random_speed(rd);
	setDir(vec3(random_dir(rd), random_dir(rd), 0.0f));
	prevPos = vPos;
	size = random_size(rd);

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
	if (moveType == (GLint)MOVE_TYPE::BOUNCING) {
		vPos.x += dir.x;
		vPos.y += dir.y;
		if (vPos.x > 1.0f - size || vPos.x < -1.0f + size) dir.x = -dir.x;
		if (vPos.y > 1.0f - (3 * size) || vPos.y < -1.0f) dir.y = -dir.y;
	}
	if(moveType == (GLint)MOVE_TYPE::ZIGZAG_HORIZONTAL) {
		if (vPos.x <= 1.0f - size && vPos.x >= -1.0f + size) {
			vPos.x += dir.x;
		}
		else {
			vPos.y += dir.y;
			if (abs((prevPos.y + 1.0f) - (vPos.y + 1.0f)) >= 0.2f) {
				dir.x = -dir.x;
				vPos.x += dir.x;
				prevPos.y = vPos.y;
			}
		}
		if (vPos.y > 1.0f - (3 * size) || vPos.y < -1.0f) {
			dir.y = -dir.y;
			prevPos.y = vPos.y;
		}
	}
	GLfloat triangleVertices[] = {
		// Triangle vertices
		vPos.x - size, vPos.y, vPos.z,			vColor.x, vColor.y, vColor.z,
		vPos.x + size, vPos.y, vPos.z,			vColor.x, vColor.y, vColor.z,
		vPos.x, vPos.y + (3 * size), vPos.z,    vColor.x, vColor.y, vColor.z
	};
	upload(triangleVertices, sizeof(triangleVertices));
}
