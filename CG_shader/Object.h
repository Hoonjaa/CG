#include "pch.h"

class Object {
private:
	GLfloat pos_x, pos_y;
	GLfloat r, g, b;
	GLuint VAO, VBO;

public:
	virtual GLvoid init() = 0;				//모양, 색, 위치 초기화
	virtual GLvoid draw() = 0;				//그리기
	virtual GLvoid update() = 0;			//위치 변경
};