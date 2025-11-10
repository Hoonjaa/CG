#pragma once
#include "Object.h"
class Hexahedron : public Object
{
private:
	GLfloat vertices[6 * 8] = {
		vPos.x + 0.5f, vPos.y + 0.5f, vPos.z + 0.5f,  vColor.r, vColor.g, vColor.b,
		vPos.x - 0.5f, vPos.y + 0.5f, vPos.z + 0.5f,  vColor.r, vColor.g, vColor.b,
		vPos.x - 0.5f, vPos.y - 0.5f, vPos.z + 0.5f,  vColor.r, vColor.g, vColor.b,
		vPos.x + 0.5f, vPos.y - 0.5f, vPos.z + 0.5f,  vColor.r, vColor.g, vColor.b,
		vPos.x - 0.5f, vPos.y + 0.5f, vPos.z - 0.5f,  vColor.r, vColor.g, vColor.b,
		vPos.x - 0.5f, vPos.y - 0.5f, vPos.z - 0.5f,  vColor.r, vColor.g, vColor.b,
		vPos.x + 0.5f, vPos.y + 0.5f, vPos.z - 0.5f,  vColor.r, vColor.g, vColor.b,
		vPos.x + 0.5f, vPos.y - 0.5f, vPos.z - 0.5f,  vColor.r, vColor.g, vColor.b,
	};
	GLuint indexes[6 * 6] = {
		0,1,2,  0,2,3,       // ¾Õ¸é
		1,4,5,  1,5,2,       // ¿ÞÂÊ¸é
		4,6,7,  4,7,5,       // µÞ¸é
		6,0,3,  6,3,7,       // ¿À¸¥ÂÊ¸é
		6,4,1,  6,1,0,       // À­¸é
		7,5,2,  7,2,3        // ¾Æ·§¸é
	};
public:
	GLint focus[6] = { 0,1,2,3,4,5 };
	GLfloat upper_angle = 0.0f;
	GLfloat y_rotate_angle = 0.0f;
	GLfloat front_angle = 0.0f;
	GLfloat side_angle = 0.0f;
	GLfloat back_size = 1.0f;
public:
	Hexahedron();
	virtual ~Hexahedron();

	GLvoid update_vertex();
	GLvoid update_matrix(const GLuint& ShaderID, const glm::mat4& main_matirx) {
		glm::mat4 final_matrix = main_matirx * modelMatrix;
		unsigned int modelLocation = glGetUniformLocation(ShaderID, "Transform");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(final_matrix));
	}

	GLvoid draw(const GLuint& ShaderID, const glm::mat4& main_matirx) override;
	GLvoid update() override;
};
