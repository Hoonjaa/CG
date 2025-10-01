#define _CRT_SECURE_NO_WARNINGS											//--- 프로그램 맨 앞에 선언할 것
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

struct vec2 {
	GLfloat x, y;
	vec2(GLfloat x = 0, GLfloat y = 0) : x(x), y(y) {}
};

//--- 아래 5개 함수는 사용자 정의 함수임
void make_vertexShaders();
void make_fragmentShaders();
GLuint make_shaderProgram();
GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid InitBuffer();

GLvoid Mouse(int button, int state, int x, int y);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Win_to_GL_mouse(int x, int y, GLfloat& gl_x, GLfloat& gl_y);
GLvoid add_point(GLfloat x, GLfloat y);
GLvoid add_line(GLfloat x, GLfloat y);
GLvoid add_triangle(GLfloat x, GLfloat y);
GLvoid add_rectangle(GLfloat x, GLfloat y);
GLvoid clear_objects();
GLvoid change_mode(unsigned char key);

//--- 필요한 변수 선언
GLint width, height;
GLuint shaderProgramID;													//--- 세이더 프로그램 이름
GLuint vertexShader;													//--- 버텍스 세이더 객체
GLuint fragmentShader;													//--- 프래그먼트 세이더 객체

GLint WindowWidth = 500, WindowHeight = 500;
GLuint max_objects = 10;
GLuint object_count = 0;
bool vertex_mode = true;
bool line_mode = false;
bool triangle_mode = false;
bool rect_mode = false;
std::vector<vec2> points;
std::vector<vec2> lines;
std::vector<vec2> triangles;
std::vector<vec2> rects;

GLuint pVAO, pVBO;
GLuint lVAO, lVBO;
GLuint tVAO, tVBO;
GLuint rVAO, rVBO;

int selected_point = -1;
int selected_line = -1;
int selected_triangle = -1;
int selected_rect = -1;

char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;
	fptr = fopen(file, "rb");											// Open file for reading
	if (!fptr)															// Return NULL on failure
		return NULL;
	fseek(fptr, 0, SEEK_END);											// Seek to the end of the file
	length = ftell(fptr);												// Find out how many bytes into the file we are
	buf = (char*)malloc(length + 1);									// Allocate a buffer for the entire length of the file and a null terminator
	fseek(fptr, 0, SEEK_SET);											// Go back to the beginning of the file
	fread(buf, length, 1, fptr);										// Read the contents of the file in to the buffer
	fclose(fptr);														// Close the file
	buf[length] = 0;													// Null terminator
	return buf;															// Return the buffer
}
//--- 메인 함수
void main(int argc, char** argv)										//--- 윈도우 출력하고 콜백함수 설정
{
	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WindowWidth, WindowHeight);
	glutCreateWindow("Example1");
	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();
	//--- 세이더 읽어와서 세이더 프로그램 만들기: 사용자 정의함수 호출
	make_vertexShaders();												//--- 버텍스 세이더 만들기
	make_fragmentShaders();												//--- 프래그먼트 세이더 만들기
	shaderProgramID = make_shaderProgram();
	InitBuffer();
	//--- 세이더 프로그램 만들기
	glutDisplayFunc(drawScene);											//--- 출력 콜백 함수
	glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard);
	glutMainLoop();
}

//--- 버텍스 세이더 객체 만들기
void make_vertexShaders()
{
	GLchar* vertexSource;
	//--- 버텍스 세이더 읽어 저장하고 컴파일 하기
	//--- filetobuf: 사용자정의 함수로 텍스트를 읽어서 문자열에 저장하는 함수
	vertexSource = filetobuf("vertex_8.glsl");
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

//--- 프래그먼트 세이더 객체 만들기
void make_fragmentShaders()
{
	GLchar* fragmentSource;
	//--- 프래그먼트 세이더 읽어 저장하고 컴파일하기
	fragmentSource = filetobuf("fragment_8.glsl");						// 프래그세이더 읽어오기
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: frag_shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

//--- 세이더 프로그램 만들고 세이더 객체 링크하기
GLuint make_shaderProgram()
{
	GLint result;
	GLchar* errorLog = NULL;
	GLuint shaderID;
	shaderID = glCreateProgram();										//--- 세이더 프로그램 만들기
	glAttachShader(shaderID, vertexShader);								//--- 세이더 프로그램에 버텍스 세이더 붙이기
	glAttachShader(shaderID, fragmentShader);							//--- 세이더 프로그램에 프래그먼트 세이더 붙이기
	glLinkProgram(shaderID);											//--- 세이더 프로그램 링크하기
	glDeleteShader(vertexShader);										//--- 세이더 객체를 세이더 프로그램에 링크했음으로, 세이더 객체 자체는 삭제 가능
	glDeleteShader(fragmentShader);
	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);					// ---세이더가 잘 연결되었는지 체크하기
	if (!result) {
		glGetProgramInfoLog(shaderID, 512, NULL, errorLog);
		std::cerr << "ERROR: shader program 연결 실패\n" << errorLog << std::endl;
		return false;
	}
	glUseProgram(shaderID);												//--- 만들어진 세이더 프로그램 사용하기
	//--- 여러 개의 세이더프로그램 만들 수 있고, 그 중 한개의 프로그램을 사용하려면
	//--- glUseProgram 함수를 호출하여 사용 할 특정 프로그램을 지정한다.
	//--- 사용하기 직전에 호출할 수 있다.
	return shaderID;
}

GLvoid InitBuffer() {
	// 점 VAO, VBO
	glGenVertexArrays(1, &pVAO);
	glBindVertexArray(pVAO);

	glGenBuffers(1, &pVBO);
	glBindBuffer(GL_ARRAY_BUFFER, pVBO);
	glBufferData(GL_ARRAY_BUFFER, max_objects * sizeof(vec2), nullptr, GL_DYNAMIC_DRAW);

	GLint posAttrib = glGetAttribLocation(shaderProgramID, "loc");
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(posAttrib);
	// 선 VAO, VBO
	glGenVertexArrays(1, &lVAO);
	glBindVertexArray(lVAO);

	glGenBuffers(1, &lVBO);
	glBindBuffer(GL_ARRAY_BUFFER, lVBO);
	glBufferData(GL_ARRAY_BUFFER, max_objects * 2 * sizeof(vec2), nullptr, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(posAttrib);
	// 삼각형 VAO, VBO
	glGenVertexArrays(1, &tVAO);
	glBindVertexArray(tVAO);

	glGenBuffers(1, &tVBO);
	glBindBuffer(GL_ARRAY_BUFFER, tVBO);
	glBufferData(GL_ARRAY_BUFFER, max_objects * 3 * sizeof(vec2), nullptr, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(posAttrib);
	// 사각형 VAO, VBO, EBO
	glGenVertexArrays(1, &rVAO);
	glBindVertexArray(rVAO);

	glGenBuffers(1, &rVBO);
	glBindBuffer(GL_ARRAY_BUFFER, rVBO);
	glBufferData(GL_ARRAY_BUFFER, max_objects * 6 * sizeof(vec2), nullptr, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(posAttrib);
}

//--- 출력 콜백 함수
GLvoid drawScene()														//--- 콜백 함수: 그리기 콜백 함수
{
	GLfloat rColor, gColor, bColor;
	rColor = gColor = 1.0;
	bColor = 1.0;														//--- 배경색을 파랑색으로 설정
	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(shaderProgramID);
	glPointSize(5.0);

	glBindVertexArray(pVAO);
	glDrawArrays(GL_POINTS, 0, points.size());

	glBindVertexArray(lVAO);
	glDrawArrays(GL_LINES, 0, lines.size() * 2);

	glBindVertexArray(tVAO);
	glDrawArrays(GL_TRIANGLES, 0, triangles.size() * 3);

	glBindVertexArray(rVAO);
	glDrawArrays(GL_TRIANGLES, 0, rects.size() * 6);
	

	glutSwapBuffers();													// 화면에 출력하기
}
//--- 다시그리기 콜백 함수
GLvoid Reshape(int w, int h)											//--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}

GLvoid Mouse(int button, int state, int x, int y) {
	GLfloat gl_x, gl_y;
	Win_to_GL_mouse(x, y, gl_x, gl_y);
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (vertex_mode)
			add_point(gl_x, gl_y);
		else if (line_mode)
			add_line(gl_x, gl_y);
		else if (triangle_mode)
			add_triangle(gl_x, gl_y);
		else if (rect_mode)
			add_rectangle(gl_x, gl_y);
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		selected_point = selected_line = selected_triangle = selected_rect = -1;
		float min_dist = 0.2f;

			for (int i = 0; i < points.size(); ++i) {
				float dx = points[i].x - gl_x;
				float dy = points[i].y - gl_y;
				float dist = sqrt(dx * dx + dy * dy);
				if (dist < min_dist) {
					min_dist = dist;
					selected_point = i;
				}
			}
			for (int i = 0; i < lines.size(); ++i) {
				float x1 = lines[i].x, y1 = lines[i].y;
				float x2 = x1 + 0.1f, y2 = y1 + 0.1f;
				float mx = (x1 + x2) / 2, my = (y1 + y2) / 2;
				float dx = mx - gl_x, dy = my - gl_y;
				float dist = sqrt(dx * dx + dy * dy);
				if (dist < min_dist) {
					min_dist = dist;
					selected_line = i;
				}
			}
			for (int i = 0; i < triangles.size(); ++i) {
				float dx = triangles[i].x - gl_x;
				float dy = (triangles[i].y + 0.1f) - gl_y;
				float dist = sqrt(dx * dx + dy * dy);
				if (dist < min_dist) {
					min_dist = dist;
					selected_triangle = i;
				}
			}
			for (int i = 0; i < rects.size(); ++i) {
				float cx = rects[i].x + 0.1f;
				float cy = rects[i].y - 0.1f;
				float dx = cx - gl_x;
				float dy = cy - gl_y;
				float dist = sqrt(dx * dx + dy * dy);
				if (dist < min_dist) {
					min_dist = dist;
					selected_rect = i;
				}
			}
	}
	glutPostRedisplay();
}

GLvoid Win_to_GL_mouse(int x, int y, GLfloat& gl_x, GLfloat& gl_y) {
	gl_x = (x / (float)WindowWidth) * 2.0f - 1.0f;
	gl_y = 1.0f - (y / (float)WindowHeight) * 2.0f;
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	float delta = 0.02f;
	bool moved = false;

	if ( selected_point != -1) {
		if (key == 'w') { points[selected_point].y += delta; moved = true; }
		if (key == 's') { points[selected_point].y -= delta; moved = true; }
		if (key == 'a') { points[selected_point].x -= delta; moved = true; }
		if (key == 'd') { points[selected_point].x += delta; moved = true; }
		if (key == 'i') { points[selected_point].x -= delta; points[selected_point].y += delta; moved = true; }
		if (key == 'j') { points[selected_point].x += delta; points[selected_point].y += delta; moved = true; }
		if (key == 'k') { points[selected_point].x -= delta; points[selected_point].y -= delta; moved = true; }
		if (key == 'l') { points[selected_point].x += delta; points[selected_point].y -= delta; moved = true; }
		if (moved) {
			glBindBuffer(GL_ARRAY_BUFFER, pVBO);
			glBufferSubData(GL_ARRAY_BUFFER, selected_point * sizeof(vec2), sizeof(vec2), &points[selected_point]);
		}
	}
	else if ( selected_line != -1) {
		if (key == 'w') { lines[selected_line].y += delta; moved = true; }
		if (key == 's') { lines[selected_line].y -= delta; moved = true; }
		if (key == 'a') { lines[selected_line].x -= delta; moved = true; }
		if (key == 'd') { lines[selected_line].x += delta; moved = true; }
		if (key == 'i') { lines[selected_line].x -= delta; lines[selected_line].y += delta; moved = true; }
		if (key == 'j') { lines[selected_line].x += delta; lines[selected_line].y += delta; moved = true; }
		if (key == 'k') { lines[selected_line].x -= delta; lines[selected_line].y -= delta; moved = true; }
		if (key == 'l') { lines[selected_line].x += delta; lines[selected_line].y -= delta; moved = true; }
		if (moved) {
			glBindBuffer(GL_ARRAY_BUFFER, lVBO);
			glBufferSubData(GL_ARRAY_BUFFER, selected_line * 2 * sizeof(vec2), sizeof(vec2), &lines[selected_line]);
			vec2 second_point = vec2(lines[selected_line].x + 0.1f, lines[selected_line].y + 0.1f);
			glBufferSubData(GL_ARRAY_BUFFER, selected_line * 2 * sizeof(vec2) + sizeof(vec2), sizeof(vec2), &second_point);
		}
	}
	else if ( selected_triangle != -1) {
		if (key == 'w') { triangles[selected_triangle].y += delta; moved = true; }
		if (key == 's') { triangles[selected_triangle].y -= delta; moved = true; }
		if (key == 'a') { triangles[selected_triangle].x -= delta; moved = true; }
		if (key == 'd') { triangles[selected_triangle].x += delta; moved = true; }
		if (key == 'i') { triangles[selected_triangle].x -= delta; triangles[selected_triangle].y += delta; moved = true; }
		if (key == 'j') { triangles[selected_triangle].x += delta; triangles[selected_triangle].y += delta; moved = true; }
		if (key == 'k') { triangles[selected_triangle].x -= delta; triangles[selected_triangle].y -= delta; moved = true; }
		if (key == 'l') { triangles[selected_triangle].x += delta; triangles[selected_triangle].y -= delta; moved = true; }
		if (moved) {
			vec2 first = triangles[selected_triangle];
			vec2 second = vec2(first.x - 0.075f, first.y - 0.15f);
			vec2 third = vec2(first.x + 0.075f, first.y - 0.15f);
			vec2 tri[3] = { first, second, third };
			glBindBuffer(GL_ARRAY_BUFFER, tVBO);
			glBufferSubData(GL_ARRAY_BUFFER, selected_triangle * 3 * sizeof(vec2), sizeof(tri), tri);
		}
	}
	else if ( selected_rect != -1) {
		if (key == 'w') { rects[selected_rect].y += delta; moved = true; }
		if (key == 's') { rects[selected_rect].y -= delta; moved = true; }
		if (key == 'a') { rects[selected_rect].x -= delta; moved = true; }
		if (key == 'd') { rects[selected_rect].x += delta; moved = true; }
		if (key == 'i') { rects[selected_rect].x -= delta; rects[selected_rect].y += delta; moved = true; }
		if (key == 'j') { rects[selected_rect].x += delta; rects[selected_rect].y += delta; moved = true; }
		if (key == 'k') { rects[selected_rect].x -= delta; rects[selected_rect].y -= delta; moved = true; }
		if (key == 'l') { rects[selected_rect].x += delta; rects[selected_rect].y -= delta; moved = true; }
		if (moved) {
			vec2 first = rects[selected_rect];
			vec2 second = vec2(first.x, first.y - 0.2f);
			vec2 third = vec2(first.x + 0.2f, first.y - 0.2f);
			vec2 fourth = vec2(first.x + 0.2f, first.y);
			vec2 rect[6] = { first, second, third, third, fourth, first };
			glBindBuffer(GL_ARRAY_BUFFER, rVBO);
			glBufferSubData(GL_ARRAY_BUFFER, selected_rect * 6 * sizeof(vec2), sizeof(rect), rect);
		}
	}
	switch (key) {
	case 'p':
		change_mode(key);
		break;
	case 'l':
		change_mode(key);
		break;
	case 't':
		change_mode(key);
		break;
	case 'r':
		change_mode(key);
		break;
	case 'c':
		clear_objects();
		break;
	}
	glutPostRedisplay();
}

GLvoid add_point(GLfloat x, GLfloat y) {
	if (object_count >= max_objects) return;
	points.push_back(vec2(x, y));
	object_count++;
	glBindBuffer(GL_ARRAY_BUFFER, pVBO);
	glBufferSubData(GL_ARRAY_BUFFER, (points.size() - 1) * sizeof(vec2), sizeof(vec2), &points.back());
}

GLvoid add_line(GLfloat x, GLfloat y) {
	if (object_count >= max_objects) return;
	lines.push_back(vec2(x, y));
	vec2 second_point = vec2(x + 0.1f, y + 0.1f);
	object_count++;
	glBindBuffer(GL_ARRAY_BUFFER, lVBO);
	glBufferSubData(GL_ARRAY_BUFFER, (lines.size() - 1) * 2 * sizeof(vec2), sizeof(vec2), &lines.back());
	glBufferSubData(GL_ARRAY_BUFFER, (lines.size() - 1) * 2 * sizeof(vec2) + sizeof(vec2), sizeof(vec2), &second_point);
}

GLvoid add_triangle(GLfloat x, GLfloat y) {
	if (object_count >= max_objects) return;
	triangles.push_back(vec2(x, y + 0.1f));
	vec2 second_point = vec2(x - 0.075f, y - 0.05f);
	vec2 third_point = vec2(x + 0.075f, y - 0.05f);
	object_count++;
	vec2 tri[3] = { triangles.back(), second_point, third_point };
	glBindBuffer(GL_ARRAY_BUFFER, tVBO);
	glBufferSubData(GL_ARRAY_BUFFER, (triangles.size() - 1) * 3 * sizeof(vec2), sizeof(tri), tri);
}

GLvoid add_rectangle(GLfloat x, GLfloat y) {
	if (object_count >= max_objects) return;
	size_t oldSize = rects.size();
	rects.push_back(vec2(x - 0.1f, y + 0.1f));
	vec2 second_point = vec2(x - 0.1f, y - 0.1f);
	vec2 third_point = vec2(x + 0.1f, y - 0.1f);
	vec2 fourth_point = vec2(x + 0.1f, y + 0.1f);
	object_count++;
	vec2 rect[6] = { rects.back(), second_point, third_point, third_point, fourth_point, rects.back()};
	glBindBuffer(GL_ARRAY_BUFFER, rVBO);
	glBufferSubData(GL_ARRAY_BUFFER, (rects.size() - 1) * 6 * sizeof(vec2), sizeof(rect), rect);
}

GLvoid change_mode(unsigned char key) {
	vertex_mode = false; line_mode = false; triangle_mode = false; rect_mode = false;
	if (key == 'p') {
		vertex_mode = true;
	}
	else if (key == 'l') {
		line_mode = true;
	}
	else if (key == 't') {
		triangle_mode = true;
	}
	else if (key == 'r') {
		rect_mode = true;
	}
}

GLvoid clear_objects() {
	points.clear();
	lines.clear();
	triangles.clear();
	rects.clear();
	object_count = 0;
	glBindBuffer(GL_ARRAY_BUFFER, pVBO);
	glBufferData(GL_ARRAY_BUFFER, max_objects * sizeof(vec2), nullptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, lVBO);
	glBufferData(GL_ARRAY_BUFFER, max_objects * 2 * sizeof(vec2), nullptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, tVBO);
	glBufferData(GL_ARRAY_BUFFER, max_objects * 3 * sizeof(vec2), nullptr, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, rVBO);
	glBufferData(GL_ARRAY_BUFFER, max_objects * 6 * sizeof(vec2), nullptr, GL_DYNAMIC_DRAW);
}