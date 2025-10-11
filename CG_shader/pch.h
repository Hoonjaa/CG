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

extern std::random_device rd;
extern std::uniform_real_distribution<float> random_color;
extern std::uniform_real_distribution<float> random_size;
extern std::uniform_real_distribution<float> random_dir;
extern std::uniform_real_distribution<float> random_speed;
extern std::uniform_real_distribution<float> random_col;

struct vec3 {
	GLfloat x, y, z;
public:
	vec3(GLfloat x = 0, GLfloat y = 0, GLfloat z = 0) : x(x), y(y), z(z) {}

	GLfloat Length() {
		return sqrt(x * x + y * y + z * z);
	}

	vec3& Normalize() {
		GLfloat length = Length();
		if (length != 0) {
			x /= length;
			y /= length;
			z /= length;
		}
		return *this;
	}
};

enum class DRAWMODE {
	POINT = GL_POINTS,
	LINE = GL_LINES,
	LINE_STRIP = GL_LINE_STRIP,
	TRIANGLE = GL_TRIANGLES,
	LINE_LOOP = GL_LINE_LOOP,
	END
};

enum class MOVE_TYPE {
	STOP,
	BOUNCING,
	ZIGZAG_HORIZONTAL,
	SPIRAL_SQUARE,
	SPIRAL_CIRCLE,
	END
};

