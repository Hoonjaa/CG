#pragma once
#include "Object.h"
class Robot_arm : public Object
{
private:
	GLfloat vertices[6 * 8];

	GLvoid setVertexInfo() {
		GLfloat temp[48] = {
			vPos.x + 0.1f, vPos.y + 0.5f, vPos.z + 0.1f,  vColor.r, vColor.g, vColor.b,
			vPos.x - 0.1f, vPos.y + 0.5f, vPos.z + 0.1f,  vColor.r, vColor.g, vColor.b,
			vPos.x - 0.1f, vPos.y - 0.5f, vPos.z + 0.1f,  vColor.r, vColor.g, vColor.b,
			vPos.x + 0.1f, vPos.y - 0.5f, vPos.z + 0.1f,  vColor.r, vColor.g, vColor.b,
			vPos.x + 0.1f, vPos.y + 0.5f, vPos.z - 0.1f,  vColor.r, vColor.g, vColor.b,
			vPos.x - 0.1f, vPos.y + 0.5f, vPos.z - 0.1f,  vColor.r, vColor.g, vColor.b,
			vPos.x - 0.1f, vPos.y - 0.5f, vPos.z - 0.1f,  vColor.r, vColor.g, vColor.b,
			vPos.x + 0.1f, vPos.y - 0.5f, vPos.z - 0.1f,  vColor.r, vColor.g, vColor.b,
		};
		memcpy(vertices, temp, sizeof(temp));
	}

	GLuint indexes[6 * 6] = {
		0,1,2,  0,2,3,       // 앞면
		1,5,6,  1,6,2,       // 왼쪽면
		5,4,7,  5,7,6,       // 뒷면
		4,0,3,  4,3,7,       // 오른쪽면
		4,5,1,  4,1,0,       // 윗면
		3,2,6,  3,6,7        // 아랫면
	};
public:
	Robot_arm();
	~Robot_arm();

	GLvoid set_color(const glm::vec3& color) {
		vColor = color;
		setVertexInfo(); // 정점 색상 정보 업데이트

		// VBO 데이터 갱신
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		upload(vertices, sizeof(vertices));
	}

	GLvoid draw(const GLuint& ShaderID, const glm::mat4& main_matirx) override;
	GLvoid update() override;
};

