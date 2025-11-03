#include "pch.h"
#include "Square_horn.h"

Square_horn::Square_horn()
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

    setDrawMode(GL_TRIANGLES);
}

Square_horn::~Square_horn()
{
}

GLvoid Square_horn::update_vertex()
{
    const GLfloat tmp[30] = {
        vPos.x + 0.5f, vPos.y - 0.5f, vPos.z + 0.5f,  vColor.r, vColor.g, vColor.b,
        vPos.x - 0.5f, vPos.y - 0.5f, vPos.z + 0.5f,  vColor.r, vColor.g, vColor.b,
        vPos.x - 0.5f, vPos.y - 0.5f, vPos.z - 0.5f,  vColor.r, vColor.g, vColor.b,
        vPos.x + 0.5f, vPos.y - 0.5f, vPos.z - 0.5f,  vColor.r, vColor.g, vColor.b,
        vPos.x,        vPos.y + 0.5f, vPos.z,       vColor.r, vColor.g, vColor.b
    };

    // CPU 측 배열 갱신
    std::memcpy(vertices, tmp, sizeof(tmp));

    // GPU(VBO) 갱신
    upload(vertices, sizeof(vertices), 0);
}

GLvoid Square_horn::draw()
{
    glBindVertexArray(VAO);
    vColor = glm::vec3(1.0f, 0.5f, 0.5f);
	update_vertex();
    glDrawElements(DrawMode, 6, GL_UNSIGNED_INT, nullptr);
    for (GLint i = 2; i < 6; i++) {
		if (focus_face[i - 2] + 2 != i) continue;

        if (i == 2) vColor = glm::vec3(1.0f, 0.0f, 1.0f);
        else  if (i == 3) vColor = glm::vec3(1.0f, 0.5f, 0.0f);
        else  if (i == 4) vColor = glm::vec3(0.0f, 0.5f, 1.0f);
        else  if (i == 5) vColor = glm::vec3(1.0f, 0.0f, 0.0f);

        update_vertex();
        const GLsizeiptr byteOffset = static_cast<GLsizeiptr>(i) * 3 * sizeof(GLuint);
        glDrawElements(DrawMode, 3, GL_UNSIGNED_INT, reinterpret_cast<void*>(byteOffset));
    }
}

GLvoid Square_horn::update()
{
    return GLvoid();
}
