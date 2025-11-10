#include "pch.h"
#include "Hexahedron.h"

Hexahedron::Hexahedron()
{
    // VBO: 정점 데이터
    allocate(sizeof(vertices), GL_STATIC_DRAW);
    upload(vertices, sizeof(vertices));

    // 위치(0), 색상(1)
    setVertexAttrib(0, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void*)0);
    setVertexAttrib(1, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    // EBO: 인덱스 데이터
    allocateIndex(sizeof(indexes), GL_STATIC_DRAW);
    uploadIndex(indexes, sizeof(indexes));

    vertexCount = 12;
    setDrawMode(GL_TRIANGLES);
}

Hexahedron::~Hexahedron()
{
}

GLvoid Hexahedron::draw(const GLuint& ShaderID, const glm::mat4& main_matirx)
{
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBindVertexArray(VAO);
    for (GLint i = 0; i < 6; i++) {
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::rotate(modelMatrix, glm::radians(y_rotate_angle), glm::vec3(0.0f, 1.0f, 0.0f));
        if (focus[i] != i) continue;

        if (i == 0) {
            vColor = glm::vec3(1.0f, 1.0f, 0.0f); // 앞면 노랑
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.5f, 0.5f));
            modelMatrix = glm::rotate(modelMatrix, glm::radians(front_angle), glm::vec3(-1.0f, 0.0f, 0.0f));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -0.5f, -0.5f));
        }
        else  if (i == 1) {
            vColor = glm::vec3(0.0f, 1.0f, 0.0f); // 왼쪽면 초록
            modelMatrix = glm::rotate(modelMatrix, glm::radians(side_angle), glm::vec3(1.0f, 0.0f, 0.0f));
        }
        else  if (i == 2) {
            vColor = glm::vec3(1.0f, 0.0f, 1.0f); // 뒷면 보라
			modelMatrix = glm::scale(modelMatrix, glm::vec3(back_size, back_size, 1.0f));
        }
        else  if (i == 3) {
            vColor = glm::vec3(1.0f, 0.5f, 0.0f); // 오른쪽면 주황
            modelMatrix = glm::rotate(modelMatrix, glm::radians(side_angle), glm::vec3(1.0f, 0.0f, 0.0f));
        }
        else  if (i == 4) {
            vColor = glm::vec3(0.0f, 0.5f, 1.0f); // 윗면 하늘
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.5f, 0.0f));
			modelMatrix = glm::rotate(modelMatrix, glm::radians(upper_angle), glm::vec3(0.0f, 0.0f, 1.0f));
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -0.5f, 0.0f));
        }
        else  if (i == 5) vColor = glm::vec3(1.0f, 0.0f, 0.0f); // 아랫면 빨강

        update_vertex();
        update_matrix(ShaderID, main_matirx);
        const GLsizeiptr byteOffset = static_cast<GLsizeiptr>(i) * 6 * sizeof(GLuint);
        glDrawElements(DrawMode, 6, GL_UNSIGNED_INT, reinterpret_cast<void*>(byteOffset));
    }
	modelMatrix = glm::mat4(1.0f);
	update_matrix(ShaderID, main_matirx);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

GLvoid Hexahedron::update()
{
    return GLvoid();
}

GLvoid Hexahedron::update_vertex()
{
    const GLfloat tmp[48] = {
        vPos.x + 0.5f, vPos.y + 0.5f, vPos.z + 0.5f,  vColor.r, vColor.g, vColor.b,
        vPos.x - 0.5f, vPos.y + 0.5f, vPos.z + 0.5f,  vColor.r, vColor.g, vColor.b,
        vPos.x - 0.5f, vPos.y - 0.5f, vPos.z + 0.5f,  vColor.r, vColor.g, vColor.b,
        vPos.x + 0.5f, vPos.y - 0.5f, vPos.z + 0.5f,  vColor.r, vColor.g, vColor.b,
        vPos.x - 0.5f, vPos.y + 0.5f, vPos.z - 0.5f,  vColor.r, vColor.g, vColor.b,
        vPos.x - 0.5f, vPos.y - 0.5f, vPos.z - 0.5f,  vColor.r, vColor.g, vColor.b,
        vPos.x + 0.5f, vPos.y + 0.5f, vPos.z - 0.5f,  vColor.r, vColor.g, vColor.b,
        vPos.x + 0.5f, vPos.y - 0.5f, vPos.z - 0.5f,  vColor.r, vColor.g, vColor.b,
    };

    // CPU 측 배열 갱신
    std::memcpy(vertices, tmp, sizeof(tmp));

    // GPU(VBO) 갱신
    upload(vertices, sizeof(vertices), 0);
}