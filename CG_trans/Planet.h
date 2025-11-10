#pragma once
#include "Object.h"

class Planet : public Object
{
	
public:
	Planet();
	virtual ~Planet();

	GLvoid draw(const GLuint& ShaderID, const glm::mat4& main_matirx) override;
	GLvoid update() override;
};

