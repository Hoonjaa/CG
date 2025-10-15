#pragma once

#define _CRT_SECURE_NO_WARNINGS											//--- 프로그램 맨 앞에 선언할 것
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <random>
#include <vector>
#include <math.h>
#include <cmath>

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/type_ptr.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>

extern std::random_device rd;
extern std::uniform_real_distribution<float> random_color;
extern std::uniform_real_distribution<float> random_size;
extern std::uniform_real_distribution<float> random_dir;
extern std::uniform_real_distribution<float> random_speed;
extern std::uniform_real_distribution<float> random_col;
extern std::uniform_real_distribution<float> random_col13;

enum class DRAWMODE {
	POINT = GL_POINTS,
	LINE = GL_LINES,
	LINE_STRIP = GL_LINE_STRIP,
	TRIANGLE = GL_TRIANGLES,
	LINE_LOOP = GL_LINE_LOOP,
	END
};