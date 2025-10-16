#include "pch.h"
#include "Hexahedron.h"

Hexahedron::Hexahedron()
{
	vColor = glm::vec3(0.5f, 0.5f, 0.0f);
	update_vertex();

    // VBO: 정점 데이터
    allocate(sizeof(vertices), GL_STATIC_DRAW);
    upload(vertices, sizeof(vertices));

    // 위치(0), 색상(1)
    setVertexAttrib(0, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void*)0);
    setVertexAttrib(1, 3, GL_FLOAT, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

    // EBO: 인덱스 데이터
    allocateIndex(sizeof(indexes), GL_STATIC_DRAW);
    uploadIndex(indexes, sizeof(indexes));

    vertexCount = 6;
    setDrawMode(GL_TRIANGLES);
}

Hexahedron::~Hexahedron()
{
}

GLvoid Hexahedron::draw()
{
    glBindVertexArray(VAO);
    glDrawElements(DrawMode, vertexCount, GL_UNSIGNED_INT, nullptr);
}

GLvoid Hexahedron::update()
{
	return GLvoid();
}

GLvoid Hexahedron::update_vertex()
{
    const GLfloat tmp[24] = {
        vPos.x + 0.5f, vPos.y + 0.5f, vPos.z + 0.5f,  vColor.r, vColor.g, vColor.b,
        vPos.x - 0.5f, vPos.y + 0.5f, vPos.z + 0.5f,  vColor.r, vColor.g, vColor.b,
        vPos.x - 0.5f, vPos.y - 0.5f, vPos.z + 0.5f,  vColor.r, vColor.g, vColor.b,
        vPos.x + 0.5f, vPos.y - 0.5f, vPos.z + 0.5f,  vColor.r, vColor.g, vColor.b,
    };

    // CPU 측 배열 갱신
    std::memcpy(vertices, tmp, sizeof(tmp));

    // GPU(VBO) 갱신
    upload(vertices, sizeof(vertices), 0);
}