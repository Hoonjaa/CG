#include "pch.h"
#include "Stage.h"

Stage::Stage()
{
	setVertexInfo();

	// VBO: 정점 데이터
	allocate(sizeof(vertices), GL_STATIC_DRAW);
	upload(vertices, sizeof(vertices));

	// 위치(0), 색상(1)
	setVertexAttrib(0, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void*)0);
	setVertexAttrib(1, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	// EBO: 인덱스 데이터
	allocateIndex(sizeof(indexes), GL_STATIC_DRAW);
	uploadIndex(indexes, sizeof(indexes));

	setDrawMode(GL_TRIANGLES);
}

Stage::~Stage()
{
}

GLvoid Stage::draw(const GLuint& ShaderID, const glm::mat4& main_matirx)
{
    glBindVertexArray(VAO);
    for (GLint i = 0; i < 6; i++) {
        modelMatrix = glm::mat4(1.0f);
        if (focus[i] != i) continue;

        if (i == 0) {
            vColor = glm::vec3(0.3f, 0.3f, 0.3f); // 앞면
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 5.0f, 5.0f));
            modelMatrix = glm::rotate(modelMatrix, glm::radians(front_angle), glm::vec3(-1.0f, 0.0f, 0.0f));
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -5.0f, -5.0f));
        }
        else  if (i == 1) {
            vColor = glm::vec3(0.5f, 0.5f, 0.5f); // 왼쪽면
        }
        else  if (i == 2) {
            vColor = glm::vec3(0.3f, 0.3f, 0.3f); // 뒷면
        }
        else  if (i == 3) {
            vColor = glm::vec3(0.5f, 0.5f, 0.5f); // 오른쪽면
        }
        else  if (i == 4) {
            vColor = glm::vec3(0.7f, 0.7f, 0.7f); // 윗면
        }
        else  if (i == 5) vColor = glm::vec3(0.7f, 0.7f, 0.7f); // 아랫면

        setVertexInfo();
		upload(vertices, sizeof(vertices));
        update_matrix(ShaderID, main_matirx);
        const GLsizeiptr byteOffset = static_cast<GLsizeiptr>(i) * 6 * sizeof(GLuint);
        glDrawElements(DrawMode, 6, GL_UNSIGNED_INT, reinterpret_cast<void*>(byteOffset));
    }
    modelMatrix = glm::mat4(1.0f);
    update_matrix(ShaderID, main_matirx);
}

GLvoid Stage::update()
{
	return GLvoid();
}
