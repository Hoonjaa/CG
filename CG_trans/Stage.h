#pragma once
#include "Object.h"
class Stage : public Object
{
private:
	GLfloat vertices[6 * 8];

	GLvoid setVertexInfo() {
		GLfloat temp[48] = {
			vPos.x + 3.0f, vPos.y + 3.0f, vPos.z + 3.0f,  vColor.r, vColor.g, vColor.b,
			vPos.x - 3.0f, vPos.y + 3.0f, vPos.z + 3.0f,  vColor.r, vColor.g, vColor.b,
			vPos.x - 3.0f, vPos.y - 3.0f, vPos.z + 3.0f,  vColor.r, vColor.g, vColor.b,
			vPos.x + 3.0f, vPos.y - 3.0f, vPos.z + 3.0f,  vColor.r, vColor.g, vColor.b,
			vPos.x + 3.0f, vPos.y + 3.0f, vPos.z - 3.0f,  vColor.r, vColor.g, vColor.b,
			vPos.x - 3.0f, vPos.y + 3.0f, vPos.z - 3.0f,  vColor.r, vColor.g, vColor.b,
			vPos.x - 3.0f, vPos.y - 3.0f, vPos.z - 3.0f,  vColor.r, vColor.g, vColor.b,
			vPos.x + 3.0f, vPos.y - 3.0f, vPos.z - 3.0f,  vColor.r, vColor.g, vColor.b,
		};
		memcpy(vertices, temp, sizeof(temp));
	}

	GLuint indexes[6 * 6] = {
		0,1,2,  0,2,3,       // ¾Õ¸é
		1,5,6,  1,6,2,       // ¿ÞÂÊ¸é
		5,4,7,  5,7,6,       // µÞ¸é
		4,0,3,  4,3,7,       // ¿À¸¥ÂÊ¸é
		4,5,1,  4,1,0,       // À­¸é
		3,2,6,  3,6,7        // ¾Æ·§¸é
	};
public:
	GLint focus[6] = { 0,1,2,3,4,5 };
	GLfloat front_angle = 0.0f;

public:
	Stage();
	~Stage();

	GLvoid update_matrix(const GLuint& ShaderID, const glm::mat4& main_matirx) {
		glm::mat4 final_matrix = main_matirx * modelMatrix;
		unsigned int modelLocation = glGetUniformLocation(ShaderID, "Transform");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(final_matrix));
	}

	GLvoid draw(const GLuint& ShaderID, const glm::mat4& main_matirx) override;
	GLvoid update() override;
};

