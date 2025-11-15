#pragma once
#include "pch.h"
#include "TreeNode.h"
#include "Object.h"

// SceneNode와 Object를 연결하는 어댑터
class TankPart : public TreeNode {
protected:
    std::shared_ptr<Object> renderObject;
    GLuint currentShaderID;

public:
    TankPart(std::shared_ptr<Object> obj, GLuint shaderID)
        : renderObject(obj), currentShaderID(shaderID) {
    }

    void draw(const glm::mat4& worldTransform) override {
        if (renderObject) {
            // worldTransform을 Object::draw에 전달
            renderObject->draw(currentShaderID, worldTransform);
        }
    }

    std::shared_ptr<Object> getObject() { return renderObject; }
};