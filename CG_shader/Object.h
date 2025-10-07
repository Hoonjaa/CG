#pragma once
#include "pch.h"

class Object {
protected:
	vec3 vPos = vec3(0.0f, 0.0f, 0.0f);
	vec3 vColor = vec3(1.0f, 1.0f, 1.0f);
	GLuint VAO = 0;
	GLuint VBO = 0;
	GLint vertexCount = 0;
	GLint DrawMode = (GLint)DRAWMODE::POINT;

protected:
	// 버퍼 생성 및 크기 할당 (데이터는 비움)
	void allocate(GLsizeiptr size, GLenum usage = GL_STATIC_DRAW);
	// 데이터 업로드 (부분 갱신 가능)
	void upload(const void* data, GLsizeiptr size, GLintptr offset = 0);
	// 속성 설정
	void setVertexAttrib(GLuint index, GLint size, GLenum type,
		GLsizei stride, const void* pointer, GLboolean normalized = GL_FALSE);

public:
	Object();
	virtual ~Object();

	GLvoid setDrawMode(GLint mode) { DrawMode = mode; }

	virtual GLvoid draw() = 0;				//그리기
	virtual GLvoid update() = 0;			//위치 변경
};