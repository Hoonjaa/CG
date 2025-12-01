#pragma once
#include "pch.h"

class TreeNode {
private:
	glm::mat4 localTransform;
	std::vector<std::shared_ptr<TreeNode>> children;
	TreeNode* parent;

public:
	TreeNode() : localTransform(glm::mat4(1.0f)), parent(nullptr) {}
	virtual ~TreeNode() = default;

	GLvoid setTransform(const glm::mat4& transform) {
		localTransform = transform;
	}

	GLvoid translate(const glm::vec3& offset) {
		localTransform = glm::translate(localTransform, offset);
	}

	GLvoid rotate(GLfloat angle, const glm::vec3& axis) {
		localTransform = glm::rotate(localTransform, angle, axis);
	}

	GLvoid scale(const glm::vec3& scaleVec) {
		localTransform = glm::scale(localTransform, scaleVec);
	}

	GLvoid addChild(std::shared_ptr<TreeNode> child) {
		children.push_back(child);
		child->parent = this;
	}

	glm::mat4 getWorldTransform() const {
		if (parent) {
			return parent->getWorldTransform() * localTransform;
		}
		return localTransform;
	}

	virtual GLvoid draw(const glm::mat4& worldTransform) {
	}

	virtual GLvoid render(const glm::mat4& parentTransform = glm::mat4(1.0f)) {
		glm::mat4 worldTransform = parentTransform * localTransform;
		draw(worldTransform);
		for (auto& child : children) {
			child->render(worldTransform);
		}
	}

	const std::vector<std::shared_ptr<TreeNode>>& getChildren() const {
		return children;
	}
};