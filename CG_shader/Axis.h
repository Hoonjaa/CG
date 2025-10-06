#pragma once
#include "Object.h"
class Axis : public Object
{
public:
	Axis();
	~Axis();

	virtual GLvoid draw() override;
	virtual GLvoid update() override;
};