#pragma once
#include "Object.h"
class Tank_flag : public Object
{
private:
	GLfloat vertices[6 * 8];

	GLvoid setVertexInfo() {
		GLfloat temp[48] = {
			vPos.x + 0.1f, vPos.y + 0.4f, vPos.z + 0.1f,  vColor.r, vColor.g, vColor.b,
			vPos.x - 0.1f, vPos.y + 0.4f, vPos.z + 0.1f,  vColor.r, vColor.g, vColor.b,
			vPos.x - 0.1f, vPos.y - 0.4f, vPos.z + 0.1f,  vColor.r, vColor.g, vColor.b,
			vPos.x + 0.1f, vPos.y - 0.4f, vPos.z + 0.1f,  vColor.r, vColor.g, vColor.b,
			vPos.x + 0.1f, vPos.y + 0.4f, vPos.z - 0.1f,  vColor.r, vColor.g, vColor.b,
			vPos.x - 0.1f, vPos.y + 0.4f, vPos.z - 0.1f,  vColor.r, vColor.g, vColor.b,
			vPos.x - 0.1f, vPos.y - 0.4f, vPos.z - 0.1f,  vColor.r, vColor.g, vColor.b,
			vPos.x + 0.1f, vPos.y - 0.4f, vPos.z - 0.1f,  vColor.r, vColor.g, vColor.b,
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
	Tank_flag();
	~Tank_flag();
	GLvoid draw(const GLuint& ShaderID, const glm::mat4& main_matirx) override;
	GLvoid update() override;
};

