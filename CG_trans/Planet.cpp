#include "pch.h"
#include "Planet.h"

Planet::Planet(GLfloat r, GLuint sectors, GLuint stacks, glm::vec3 color, glm::vec3 Pos)
{
	vPos = Pos;
	vColor = color;
	setPlanetVertex(r, sectors, stacks, vColor, vPos);

	allocate(sizeof(VertexInfo) * vertices.size(), GL_STATIC_DRAW);
	upload(vertices.data(), sizeof(VertexInfo) * vertices.size());

	setVertexAttrib(0, 3, GL_FLOAT, sizeof(VertexInfo), (void*)0); // 위치
	setVertexAttrib(1, 3, GL_FLOAT, sizeof(VertexInfo), (void*)(3 * sizeof(GLfloat))); // 색상

	allocateIndex(sizeof(IndexInfo) * indexes.size(), GL_STATIC_DRAW);
	uploadIndex(indexes.data(), sizeof(IndexInfo) * indexes.size());

	setDrawMode(GL_TRIANGLES);
}

Planet::~Planet()
{
}

GLvoid Planet::draw(const GLuint& ShaderID, const glm::mat4& main_matirx)
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(VAO);
	glDrawElements(DrawMode, static_cast<GLsizei>(indexes.size() * 3), GL_UNSIGNED_INT, 0);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

GLvoid Planet::update()
{
	return GLvoid();
}

GLvoid Planet::setPlanetVertex(GLfloat r, GLuint sectors, GLuint stacks, glm::vec3 color, glm::vec3 Pos)
{
	radius = r;
	vertices.clear();
	indexes.clear();

	const float PI = 3.14159265359f;
	float sectorStep = 2 * PI / sectors;
	float stackStep = PI / stacks;

	// 정점 생성
	for (GLuint i = 0; i <= stacks; ++i)
	{
		float stackAngle = PI / 2 - i * stackStep;  // -π/2 에서 π/2
		float xy = radius * cosf(stackAngle);
		float z = radius * sinf(stackAngle);

		for (GLuint j = 0; j <= sectors; ++j)
		{
			float sectorAngle = j * sectorStep;  // 0 에서 2π

			VertexInfo vertex;
			vertex.vPos.x = xy * cosf(sectorAngle) + vPos.x;
			vertex.vPos.y = xy * sinf(sectorAngle) + vPos.y;
			vertex.vPos.z = z + vPos.z;

			// 색상 (법선 벡터 기반으로 설정)
			glm::vec3 normal = glm::normalize(vertex.vPos - vPos);
			vertex.vColor = glm::vec3(
				(normal.x + 1.0f) * color.r,
				(normal.y + 1.0f) * color.g,
				(normal.z + 1.0f) * color.b
			);

			vertices.push_back(vertex);
		}
	}

	// 인덱스 생성
	for (GLuint i = 0; i < stacks; ++i)
	{
		GLuint k1 = i * (sectors + 1);
		GLuint k2 = k1 + sectors + 1;

		for (GLuint j = 0; j < sectors; ++j, ++k1, ++k2)
		{
			if (i != 0)
			{
				IndexInfo index1;
				index1.index[0] = k1;
				index1.index[1] = k2;
				index1.index[2] = k1 + 1;
				indexes.push_back(index1);
			}

			if (i != (stacks - 1))
			{
				IndexInfo index2;
				index2.index[0] = k1 + 1;
				index2.index[1] = k2;
				index2.index[2] = k2 + 1;
				indexes.push_back(index2);
			}
		}
	}
}

GLvoid Planet::first_planet_update()
{
	return GLvoid();
}

GLvoid Planet::second_planet_update()
{
	return GLvoid();
}

GLvoid Planet::third_planet_update()
{
	return GLvoid();
}