#pragma once
#include "pch.h"
#include "TreeNode.h"
#include "Object.h"

// TreeNode와 Object를 연결하는 어댑터
class Object_Part : public TreeNode {
protected:
    std::shared_ptr<Object> renderObject;
    GLuint currentShaderID;
    glm::mat4 viewProjMatrix;

public:
    Object_Part(std::shared_ptr<Object> obj, GLuint shaderID)
        : renderObject(obj), currentShaderID(shaderID), viewProjMatrix(1.0f) {
    }

    void setViewProjMatrix(const glm::mat4& matrix) {
        viewProjMatrix = matrix;
    }

    void draw(const glm::mat4& worldTransform) override {
        if (renderObject) {
            // worldTransform을 Object::draw에 전달
            renderObject->draw(currentShaderID, worldTransform, viewProjMatrix);
        }
    }

    std::shared_ptr<Object> getObject() { return renderObject; }
};