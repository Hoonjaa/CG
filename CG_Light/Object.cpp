#include "pch.h"
#include "Object.h"

Object::Object()
{
}

Object::~Object()
{
	if (VBO) glDeleteBuffers(1, &VBO);
	if (EBO) glDeleteBuffers(1, &EBO);
	if (VAO) glDeleteVertexArrays(1, &VAO);
}

void Object::allocate(GLsizeiptr size, GLenum usage) {
	if (!VAO) glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	if (!VBO) glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, size, nullptr, usage);
}

void Object::upload(const void* data, GLsizeiptr size, GLintptr offset) {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}

void Object::setVertexAttrib(GLuint index, GLint size, GLenum type,
	GLsizei stride, const void* pointer, GLboolean normalized) {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glEnableVertexAttribArray(index);
	glVertexAttribPointer(index, size, type, normalized, stride, pointer);
}

void Object::allocateIndex(GLsizeiptr size, GLenum usage) {
	if (!VAO) glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	if (!EBO) glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, usage);
}

void Object::uploadIndex(const void* data, GLsizeiptr size, GLintptr offset) {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
}