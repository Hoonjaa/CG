#include "pch.h"
#include "Orbit.h"

Orbit::Orbit(GLfloat r) {
	radius = r;
	setOrbitVertex();

	allocate(sizeof(VertexInfo) * vertices.size(), GL_STATIC_DRAW);
	upload(vertices.data(), sizeof(VertexInfo) * vertices.size());
	
	setVertexAttrib(0, 3, GL_FLOAT, sizeof(VertexInfo), (void*)0); // 위치
	setVertexAttrib(1, 3, GL_FLOAT, sizeof(VertexInfo), (void*)(3 * sizeof(GLfloat))); // 색상

	setDrawMode(GL_LINE_LOOP);
}

Orbit::~Orbit() {

}

GLvoid Orbit::draw(const GLuint& ShaderID, const glm::mat4& main_matirx) {
	glBindVertexArray(VAO);
	glDrawArrays(DrawMode, 0, static_cast<GLsizei>(vertices.size()));
}

GLvoid Orbit::update() {
	setOrbitVertex();
	
	// VAO 바인딩
	glBindVertexArray(VAO);
	// VBO 바인딩 및 데이터 업로드
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexInfo) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	
	// 정점 속성 재설정
	setVertexAttrib(0, 3, GL_FLOAT, sizeof(VertexInfo), (void*)0);
	setVertexAttrib(1, 3, GL_FLOAT, sizeof(VertexInfo), (void*)(3 * sizeof(GLfloat)));
}

GLvoid Orbit::setOrbitVertex() {
	vertices.clear();
	
	const float PI = 3.14159265359f;

	for (GLfloat i = 0.0f; i <= 2 * PI; i += 0.01f) {
		VertexInfo vertex;
		vertex.vPos = glm::vec3(radius * cosf(i), 0.0f, radius * sinf(i));
		vertex.vColor = glm::vec3(0.0f, 0.0f, 0.0f);

		vertices.push_back(vertex);
	}
}