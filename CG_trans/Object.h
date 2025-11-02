#pragma once
class Object
{
protected:
	glm::vec3 vPos{ 0.0f, 0.0f, 0.0f };
	glm::vec3 vColor{ 1.0f, 1.0f, 1.0f };
	glm::mat4 modelMatrix{ 1.0f };
	GLuint VAO = 0;
	GLuint VBO = 0;
	GLuint EBO = 0;
	GLint vertexCount = 0;
	GLint DrawMode = GL_POINTS;
	// GL_LINES, GL_LINE_STRIP, GL_LINE_LOOP, GL_TRIANGLES
protected:
	// 버퍼 생성 및 크기 할당 (데이터는 비움)
	void allocate(GLsizeiptr size, GLenum usage = GL_STATIC_DRAW); // GL_DYNAMIC_DRAW
	// 데이터 업로드 (부분 갱신 가능)
	void upload(const void* data, GLsizeiptr size, GLintptr offset = 0);
	// 속성 설정
	void setVertexAttrib(GLuint index, GLint size, GLenum type,
		GLsizei stride, const void* pointer, GLboolean normalized = GL_FALSE);
	// EBO 성정
	void allocateIndex(GLsizeiptr size, GLenum usage = GL_STATIC_DRAW);	// GL_DYNAMIC_DRAW
	void uploadIndex(const void* data, GLsizeiptr size, GLintptr offset = 0);

public:
	Object();
	virtual ~Object();

	GLvoid setDrawMode(GLint mode) { DrawMode = mode; }
	GLint getDrawMode() const { return DrawMode; }

	virtual GLvoid draw() = 0;
	virtual GLvoid update() = 0;
};
