#pragma once
#include "Object.h"
class Coordinate_system : public Object
{
public:
	Coordinate_system();
	virtual ~Coordinate_system();
	virtual GLvoid draw(const GLuint& ShaderID, const glm::mat4& main_matirx) override;
	virtual GLvoid update() override;
};