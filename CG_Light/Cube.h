#pragma once
#include "Object.h"
class Cube : public Object
{
private:
	GLfloat vertices[9 * 24];

	GLvoid setVertexInfo() {
		GLfloat temp[9 * 24] = {
            // 앞면 (z = +0.5, normal = 0, 0, 1)
        vPos.x + 0.5f, vPos.y + 0.5f, vPos.z + 0.5f,  vColor.r, vColor.g, vColor.b,  0.0f, 0.0f, 1.0f,
        vPos.x - 0.5f, vPos.y + 0.5f, vPos.z + 0.5f,  vColor.r, vColor.g, vColor.b,  0.0f, 0.0f, 1.0f,
        vPos.x - 0.5f, vPos.y - 0.5f, vPos.z + 0.5f,  vColor.r, vColor.g, vColor.b,  0.0f, 0.0f, 1.0f,
        vPos.x + 0.5f, vPos.y - 0.5f, vPos.z + 0.5f,  vColor.r, vColor.g, vColor.b,  0.0f, 0.0f, 1.0f,

        // 왼쪽면 (x = -0.5, normal = -1, 0, 0)
        vPos.x - 0.5f, vPos.y + 0.5f, vPos.z + 0.5f,  vColor.r, vColor.g, vColor.b,  -1.0f, 0.0f, 0.0f,
        vPos.x - 0.5f, vPos.y + 0.5f, vPos.z - 0.5f,  vColor.r, vColor.g, vColor.b,  -1.0f, 0.0f, 0.0f,
        vPos.x - 0.5f, vPos.y - 0.5f, vPos.z - 0.5f,  vColor.r, vColor.g, vColor.b,  -1.0f, 0.0f, 0.0f,
        vPos.x - 0.5f, vPos.y - 0.5f, vPos.z + 0.5f,  vColor.r, vColor.g, vColor.b,  -1.0f, 0.0f, 0.0f,

        // 뒷면 (z = -0.5, normal = 0, 0, -1)
        vPos.x - 0.5f, vPos.y + 0.5f, vPos.z - 0.5f,  vColor.r, vColor.g, vColor.b,  0.0f, 0.0f, -1.0f,
        vPos.x + 0.5f, vPos.y + 0.5f, vPos.z - 0.5f,  vColor.r, vColor.g, vColor.b,  0.0f, 0.0f, -1.0f,
        vPos.x + 0.5f, vPos.y - 0.5f, vPos.z - 0.5f,  vColor.r, vColor.g, vColor.b,  0.0f, 0.0f, -1.0f,
        vPos.x - 0.5f, vPos.y - 0.5f, vPos.z - 0.5f,  vColor.r, vColor.g, vColor.b,  0.0f, 0.0f, -1.0f,

        // 오른쪽면 (x = +0.5, normal = 1, 0, 0)
        vPos.x + 0.5f, vPos.y + 0.5f, vPos.z - 0.5f,  vColor.r, vColor.g, vColor.b,  1.0f, 0.0f, 0.0f,
        vPos.x + 0.5f, vPos.y + 0.5f, vPos.z + 0.5f,  vColor.r, vColor.g, vColor.b,  1.0f, 0.0f, 0.0f,
        vPos.x + 0.5f, vPos.y - 0.5f, vPos.z + 0.5f,  vColor.r, vColor.g, vColor.b,  1.0f, 0.0f, 0.0f,
        vPos.x + 0.5f, vPos.y - 0.5f, vPos.z - 0.5f,  vColor.r, vColor.g, vColor.b,  1.0f, 0.0f, 0.0f,

        // 윗면 (y = +0.5, normal = 0, 1, 0)
        vPos.x + 0.5f, vPos.y + 0.5f, vPos.z - 0.5f,  vColor.r, vColor.g, vColor.b,  0.0f, 1.0f, 0.0f,
        vPos.x - 0.5f, vPos.y + 0.5f, vPos.z - 0.5f,  vColor.r, vColor.g, vColor.b,  0.0f, 1.0f, 0.0f,
        vPos.x - 0.5f, vPos.y + 0.5f, vPos.z + 0.5f,  vColor.r, vColor.g, vColor.b,  0.0f, 1.0f, 0.0f,
        vPos.x + 0.5f, vPos.y + 0.5f, vPos.z + 0.5f,  vColor.r, vColor.g, vColor.b,  0.0f, 1.0f, 0.0f,

        // 아랫면 (y = -0.5, normal = 0, -1, 0)
        vPos.x + 0.5f, vPos.y - 0.5f, vPos.z + 0.5f,  vColor.r, vColor.g, vColor.b,  0.0f, -1.0f, 0.0f,
        vPos.x - 0.5f, vPos.y - 0.5f, vPos.z + 0.5f,  vColor.r, vColor.g, vColor.b,  0.0f, -1.0f, 0.0f,
        vPos.x - 0.5f, vPos.y - 0.5f, vPos.z - 0.5f,  vColor.r, vColor.g, vColor.b,  0.0f, -1.0f, 0.0f,
        vPos.x + 0.5f, vPos.y - 0.5f, vPos.z - 0.5f,  vColor.r, vColor.g, vColor.b,  0.0f, -1.0f, 0.0f,
		};
		memcpy(vertices, temp, sizeof(temp));
	}

	GLuint indexes[6 * 6] = {
        // 앞면 (normal: 0, 0, 1)
        0,1,2,    0,2,3,
        // 왼쪽면 (normal: -1, 0, 0)
        4,5,6,    4,6,7,
        // 뒷면 (normal: 0, 0, -1)
        8,9,10,   8,10,11,
        // 오른쪽면 (normal: 1, 0, 0)
        12,13,14, 12,14,15,
        // 윗면 (normal: 0, 1, 0)
        16,17,18, 16,18,19,
        // 아랫면 (normal: 0, -1, 0)
        20,21,22, 20,22,23
	};
public:
    // 여기서 회전에 관한 변수들이나 함수들 선언 가능
	// 그리고 그 변수를 활용해서 draw 함수에서 modelMatrix 갱신 가능

public:
	Cube();
	~Cube();

	GLvoid draw(const GLuint& ShaderID, const glm::mat4& worldMatrix, const glm::mat4& viewProjMatrix) override;
	GLvoid update() override;
};

