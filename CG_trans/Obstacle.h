#pragma once
#include "Object.h"
class Obstacle : public Object
{
private:
	GLfloat vertices[6 * 8];

	GLvoid setVertexInfo() {
		GLfloat temp[48] = {
			vPos.x + 0.4f, vPos.y + 0.4f, vPos.z + 0.4f,  vColor.r, vColor.g, vColor.b,
			vPos.x - 0.4f, vPos.y + 0.4f, vPos.z + 0.4f,  vColor.r, vColor.g, vColor.b,
			vPos.x - 0.4f, vPos.y - 0.4f, vPos.z + 0.4f,  vColor.r, vColor.g, vColor.b,
			vPos.x + 0.4f, vPos.y - 0.4f, vPos.z + 0.4f,  vColor.r, vColor.g, vColor.b,
			vPos.x + 0.4f, vPos.y + 0.4f, vPos.z - 0.4f,  vColor.r, vColor.g, vColor.b,
			vPos.x - 0.4f, vPos.y + 0.4f, vPos.z - 0.4f,  vColor.r, vColor.g, vColor.b,
			vPos.x - 0.4f, vPos.y - 0.4f, vPos.z - 0.4f,  vColor.r, vColor.g, vColor.b,
			vPos.x + 0.4f, vPos.y - 0.4f, vPos.z - 0.4f,  vColor.r, vColor.g, vColor.b,
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
	Obstacle();
	~Obstacle();
	GLvoid draw(const GLuint& ShaderID, const glm::mat4& main_matirx) override;
	GLvoid update() override;

	// 충돌 박스 반환 (장애물 크기: 0.8 x 0.8 x 0.8)
	glm::vec3 getMin() const { return vPos - glm::vec3(0.4f, 0.4f, 0.4f); }
	glm::vec3 getMax() const { return vPos + glm::vec3(0.4f, 0.4f, 0.4f); }
};

