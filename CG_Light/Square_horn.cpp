#include "pch.h"
#include "Square_horn.h"

Square_horn::Square_horn()
{
    vColor = glm::vec3(1.0f, 0.5f, 0.5f);
    setVertexInfo();
    
    // VBO: 정점 데이터
    allocate(sizeof(vertices), GL_STATIC_DRAW);
    upload(vertices, sizeof(vertices));

    // 위치(0), 색상(1), 법선(2)
    setVertexAttrib(0, 3, GL_FLOAT, 9 * sizeof(GLfloat), (void*)0);
    setVertexAttrib(1, 3, GL_FLOAT, 9 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    setVertexAttrib(2, 3, GL_FLOAT, 9 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));

    // EBO: 인덱스 데이터
    allocateIndex(sizeof(indexes), GL_STATIC_DRAW);
    uploadIndex(indexes, sizeof(indexes));
    
    vertexCount = 18;  // 6개 삼각형 * 3개 정점
    setDrawMode(GL_TRIANGLES);
}

Square_horn::~Square_horn()
{
}

GLvoid Square_horn::draw(const GLuint& ShaderID, const glm::mat4& worldMatrix, const glm::mat4& viewProjMatrix)
{
    glBindVertexArray(VAO);

    // modelTransform 행렬을 여기에서 변환
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(y_angle), glm::vec3(0.0f, 1.0f, 0.0f));

    // main.cpp의 주석에서도 말했듯이 조명작업을 위해(노말 위치 필요해서임), 프래그먼트 셰이더에선 뷰,투영을 행하지 않은 모델 행렬이 필요함.
    // Object.h를 보면 기본적으로 modelMatrix는 단위행렬로 초기화 되어있음.
    // modelMatrix에 객체에 필요한 변환을 다 해주고 프래그먼트 셰이더로 보내주면 됨.
    glm::mat4 finalWorldMatrix = worldMatrix * modelMatrix;
    unsigned int modelLocation = glGetUniformLocation(ShaderID, "model");
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(finalWorldMatrix));

    // 그리고 뷰,투영 행렬과 합쳐진 final_matrix는 버텍스 셰이더로 보내주면 되는거임.
    glm::mat4 finalMatrix = viewProjMatrix * finalWorldMatrix;
    unsigned int transformLocation = glGetUniformLocation(ShaderID, "Transform");
    glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(finalMatrix));

    // 그리고 나서 최종적으로 그려주는 코드.
    glDrawElements(DrawMode, vertexCount, GL_UNSIGNED_INT, 0);
}

GLvoid Square_horn::update()
{
    return GLvoid();
}
