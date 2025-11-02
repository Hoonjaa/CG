#pragma once
#include "Object.h"
class Coordinate_system : public Object
{
public:
	Coordinate_system();
	virtual ~Coordinate_system();
	virtual GLvoid draw() override;
	virtual GLvoid update() override;
};