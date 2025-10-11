#include "pch.h"
#include "Spiral.h"

GLvoid Spiral::ensureCapacity(GLint needed)
{
	if (needed <= capacity) return;

	GLint newCap = capacity > 0 ? capacity : 256;
	while (newCap < needed) newCap *= 2;

	// 새 크기로 재할당(오퍼닝: 데이터 포인터 nullptr)
	allocate(static_cast<GLsizeiptr>(newCap) * sizeof(vec3), GL_DYNAMIC_DRAW);

	// 기존 데이터 재업로드
	if (!vertices.empty()) {
		upload(vertices.data(),
			static_cast<GLsizeiptr>(vertices.size()) * sizeof(vec3),
			0);
	}

	capacity = newCap;
	// VAO의 attribute 포인터는 동일 VBO 바인딩에 대해 유지되므로 보통 재설정 불필요
}

Spiral::Spiral(GLfloat x, GLfloat y, GLint mode)
{
	vPos = vec3(x, y, 0.0f);
	DrawMode = mode;

	// 초기 용량 설정 및 버퍼/VAO 준비
	capacity = 256; // 시작 용량(필요 시 두 배씩 증가)
	allocate(static_cast<GLsizeiptr>(capacity) * sizeof(vec3), GL_DYNAMIC_DRAW);
	setVertexAttrib(0, 3, GL_FLOAT, sizeof(vec3), reinterpret_cast<const void*>(0), GL_FALSE);

	vertices.reserve(capacity);
	vertexCount = 0;
}

Spiral::~Spiral()
{
}

GLvoid Spiral::draw()
{
	glBindVertexArray(VAO);
	// 점 그리기
	glDrawArrays(DrawMode, 0, vertexCount);
}

GLvoid Spiral::update()
{
	if (radius < 0.0f) return;
	if (theta >= 6.0f * 3.14159265358979323846f) {
		vPos.x = vPos.x + (2 *radius);
		dTheta = -0.15f;
		phase = 3.14159265358979323846f; // 180도
	}
	theta += dTheta;
	radius = k * theta;
	// 아르키메데스 나선 r = k * theta
	float a = theta + phase;

	vec3 p;
	p.x = vPos.x + radius * std::cos(a);
	p.y = vPos.y + radius * std::sin(a);
	p.z = 0.0f;

	// 새 점 추가
	vertices.push_back(p);
	ensureCapacity(static_cast<GLint>(vertices.size()));

	// 방금 추가된 점만 부분 업로드
	const GLint idx = static_cast<GLint>(vertices.size()) - 1;
	upload(&vertices[idx], sizeof(vec3), static_cast<GLintptr>(idx) * sizeof(vec3));

	vertexCount = static_cast<GLint>(vertices.size());
}
