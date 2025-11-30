#pragma once
#include "Object.h"
class Square_horn : public Object
{
private:
	GLfloat vertices[9 * 17];  // 17개 정점 * 9 (position(3) + color(3) + normal(3))

	GLvoid setVertexInfo() {
		// 각 측면의 법선 벡터 계산
		// 피라미드의 각 면은 고유한 법선을 가져야 함
		
		// 측면 법선 계산 (정규화된 벡터)
		// 각 면의 두 변을 외적하여 법선 계산
		float sqrt2 = sqrtf(2.0f);
		
		GLfloat temp[9 * 17] = {
			// 바닥면 4개 정점 (y = -0.5, normal = 0, -1, 0)
			vPos.x + 0.5f, vPos.y - 0.5f, vPos.z + 0.5f,  vColor.r, vColor.g, vColor.b,  0.0f, -1.0f, 0.0f,  // 0
			vPos.x - 0.5f, vPos.y - 0.5f, vPos.z + 0.5f,  vColor.r, vColor.g, vColor.b,  0.0f, -1.0f, 0.0f,  // 1
			vPos.x - 0.5f, vPos.y - 0.5f, vPos.z - 0.5f,  vColor.r, vColor.g, vColor.b,  0.0f, -1.0f, 0.0f,  // 2
			vPos.x + 0.5f, vPos.y - 0.5f, vPos.z - 0.5f,  vColor.r, vColor.g, vColor.b,  0.0f, -1.0f, 0.0f,  // 3
			
			// 뒷면 (z = -0.5 쪽): 두 변 (1, 0, 0)과 (0.5, 1, 0.5)의 외적 = (0, -0.707, 0.707) 정규화
			vPos.x - 0.5f, vPos.y - 0.5f, vPos.z - 0.5f,  vColor.r, vColor.g, vColor.b,  0.0f, 0.707f, -0.707f,  // 4
			vPos.x + 0.5f, vPos.y - 0.5f, vPos.z - 0.5f,  vColor.r, vColor.g, vColor.b,  0.0f, 0.707f, -0.707f,  // 5
			vPos.x,        vPos.y + 0.5f, vPos.z,         vColor.r, vColor.g, vColor.b,  0.0f, 0.707f, -0.707f,  // 6
			
			// 왼쪽면 (x = -0.5 쪽): 두 변 (0, 0, -1)과 (0.5, 1, 0.5)의 외적 = (-0.707, 0.707, 0) 정규화
			vPos.x - 0.5f, vPos.y - 0.5f, vPos.z + 0.5f,  vColor.r, vColor.g, vColor.b,  -0.707f, 0.707f, 0.0f,  // 7
			vPos.x - 0.5f, vPos.y - 0.5f, vPos.z - 0.5f,  vColor.r, vColor.g, vColor.b,  -0.707f, 0.707f, 0.0f,  // 8
			vPos.x,        vPos.y + 0.5f, vPos.z,         vColor.r, vColor.g, vColor.b,  -0.707f, 0.707f, 0.0f,  // 9
			
			// 앞면 (z = +0.5 쪽): 두 변 (-1, 0, 0)과 (-0.5, 1, -0.5)의 외적 = (0, 0.707, 0.707) 정규화
			vPos.x + 0.5f, vPos.y - 0.5f, vPos.z + 0.5f,  vColor.r, vColor.g, vColor.b,  0.0f, 0.707f, 0.707f,   // 10
			vPos.x - 0.5f, vPos.y - 0.5f, vPos.z + 0.5f,  vColor.r, vColor.g, vColor.b,  0.0f, 0.707f, 0.707f,   // 11
			vPos.x,        vPos.y + 0.5f, vPos.z,         vColor.r, vColor.g, vColor.b,  0.0f, 0.707f, 0.707f,   // 12
			
			// 오른쪽면 (x = +0.5 쪽): 두 변 (0, 0, 1)과 (-0.5, 1, 0.5)의 외적 = (0.707, 0.707, 0) 정규화
			vPos.x + 0.5f, vPos.y - 0.5f, vPos.z - 0.5f,  vColor.r, vColor.g, vColor.b,  0.707f, 0.707f, 0.0f,   // 13
			vPos.x + 0.5f, vPos.y - 0.5f, vPos.z + 0.5f,  vColor.r, vColor.g, vColor.b,  0.707f, 0.707f, 0.0f,   // 14
			vPos.x,        vPos.y + 0.5f, vPos.z,         vColor.r, vColor.g, vColor.b,  0.707f, 0.707f, 0.0f    // 15
		};
		memcpy(vertices, temp, sizeof(temp));
	}

	GLuint indexes[6 * 3] = {
		// 바닥면 (2개의 삼각형) - 아래에서 봤을 때 CCW
		0,1,2,  0,2,3,
		// 뒷면 (z = -0.5 쪽) - 뒤에서 봤을 때 CCW
		4,6,5,
		// 왼쪽면 (x = -0.5 쪽) - 왼쪽에서 봤을 때 CCW
		7,9,8,
		// 앞면 (z = +0.5 쪽) - 앞에서 봤을 때 CCW
		10,12,11,
		// 오른쪽면 (x = +0.5 쪽) - 오른쪽에서 봤을 때 CCW
		13,15,14
	};

public:
	GLfloat y_angle = 0.0f;

public:
	Square_horn();
	virtual ~Square_horn();

	GLvoid draw(const GLuint& ShaderID, const glm::mat4& worldMatrix, const glm::mat4& viewProjMatrix) override;
	GLvoid update() override;
};

