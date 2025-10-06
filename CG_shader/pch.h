#pragma once

#define _CRT_SECURE_NO_WARNINGS											//--- 프로그램 맨 앞에 선언할 것
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

struct vec2 {
	GLfloat x, y;
	vec2(GLfloat x = 0, GLfloat y = 0) : x(x), y(y) {}
};