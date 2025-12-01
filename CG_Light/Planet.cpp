#include "pch.h"
#include "Planet.h"

Planet::Planet(GLfloat r, GLuint sectors, GLuint stacks, glm::vec3 color, glm::vec3 Pos)
{
	vPos = Pos;
	vColor = color;
	orbit_radius = glm::length(Pos);  // 초기 위치로부터 궤도 반지름 계산
	setPlanetVertex(r, sectors, stacks, vColor, vPos);

	allocate(sizeof(VertexInfo) * vertices.size(), GL_STATIC_DRAW);
	upload(vertices.data(), sizeof(VertexInfo) * vertices.size());

	setVertexAttrib(0, 3, GL_FLOAT, sizeof(VertexInfo), (void*)0); // 위치
	setVertexAttrib(1, 3, GL_FLOAT, sizeof(VertexInfo), (void*)(3 * sizeof(GLfloat))); // 색상
	setVertexAttrib(2, 3, GL_FLOAT, sizeof(VertexInfo), (void*)(6 * sizeof(GLfloat))); // 노멀

	allocateIndex(sizeof(IndexInfo) * indexes.size(), GL_STATIC_DRAW);
	uploadIndex(indexes.data(), sizeof(IndexInfo) * indexes.size());

	setDrawMode(GL_TRIANGLES);
}

Planet::~Planet()
{
}

GLvoid Planet::draw(const GLuint& ShaderID, const glm::mat4& worldMatrix, const glm::mat4& viewProjMatrix)
{
	if(!solid_mode) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBindVertexArray(VAO);
	
	// model matrix 설정 (worldMatrix는 부모 변환, modelMatrix는 자체 변환)
	glm::mat4 finalWorldMatrix = worldMatrix * modelMatrix;
	unsigned int modelLocation = glGetUniformLocation(ShaderID, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(finalWorldMatrix));

	// Transform matrix 설정 (최종 변환 행렬)
	glm::mat4 finalMatrix = viewProjMatrix * finalWorldMatrix;
	unsigned int transformLocation = glGetUniformLocation(ShaderID, "Transform");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(finalMatrix));
	
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

			// 노멀 벡터 계산 (구의 중심에서 정점을 향하는 단위 벡터)
			glm::vec3 normal = glm::normalize(vertex.vPos - vPos);
			vertex.vNormal = normal;

			// 색상 (전달받은 색상 직접 사용)
			vertex.vColor = color;

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
	setPlanetVertex(radius, 36, 18, vColor, vPos);

	// VAO 바인딩
	glBindVertexArray(VAO);
	// VBO 바인딩 및 데이터 업로드
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexInfo) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(IndexInfo) * indexes.size(), indexes.data(), GL_STATIC_DRAW);

	// 정점 속성 재설정
	setVertexAttrib(0, 3, GL_FLOAT, sizeof(VertexInfo), (void*)0);
	setVertexAttrib(1, 3, GL_FLOAT, sizeof(VertexInfo), (void*)(3 * sizeof(GLfloat)));
	setVertexAttrib(2, 3, GL_FLOAT, sizeof(VertexInfo), (void*)(6 * sizeof(GLfloat)));
}

GLvoid Planet::third_planet_update()
{
	return GLvoid();
}

GLvoid Planet::setOrbitRadius(GLfloat orbit_r)
{
	orbit_radius = orbit_r;
}

GLvoid Planet::updatePositionByOrbit()
{
	// X축 방향으로 궤도 반지름만큼 위치 업데이트
	vPos = glm::vec3(orbit_radius, 0.0f, 0.0f);
	second_planet_update();
}