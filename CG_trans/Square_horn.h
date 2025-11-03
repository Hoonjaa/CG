#pragma once
#include "Object.h"
class Square_horn : public Object
{
private:
	GLfloat vertices[5 * 6] = {
		vPos.x + 0.5f, vPos.y - 0.5f, vPos.z + 0.5f,  vColor.r, vColor.g, vColor.b,
		vPos.x - 0.5f, vPos.y - 0.5f, vPos.z + 0.5f,  vColor.r, vColor.g, vColor.b,
		vPos.x - 0.5f, vPos.y - 0.5f, vPos.z - 0.5f,  vColor.r, vColor.g, vColor.b,
		vPos.x + 0.5f, vPos.y - 0.5f, vPos.z - 0.5f,  vColor.r, vColor.g, vColor.b,
		vPos.x,        vPos.y + 0.5f, vPos.z,       vColor.r, vColor.g, vColor.b
	};
	GLuint indexes[6 * 3] = {
		0,2,1,  0,2,3,
		4,1,0,
		4,1,2,
		4,2,3,
		4,3,0
	};
public:
	GLint focus_face[4] = {0,1,2,3};
	GLfloat y_rotate_angle = 0.0f;
public:
	Square_horn();
	virtual ~Square_horn();

	GLvoid update_vertex();
	GLvoid update_matrix(const GLuint& ShaderID, const glm::mat4& main_matirx) {
		glm::mat4 final_matrix = main_matirx * modelMatrix;
		unsigned int modelLocation = glGetUniformLocation(ShaderID, "Transform");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(final_matrix));
	}

	GLvoid draw(const GLuint& ShaderID, const glm::mat4& main_matirx);

	GLvoid draw() override;
	GLvoid update() override;
};

