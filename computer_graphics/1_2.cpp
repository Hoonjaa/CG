#include <iostream>
#include <random>

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Mouse(int button, int state, int x, int y);
bool check_mouse_pressed_in_rect1(int x, int y);
bool check_mouse_pressed_in_rect2(int x, int y);
bool check_mouse_pressed_in_rect3(int x, int y);
bool check_mouse_pressed_in_rect4(int x, int y);
bool check_mouse_pressed_in_BG(int x, int y);

std::random_device rd;
std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

class Rect {
	GLclampf r, g, b;
	GLfloat center_x, center_y;
	GLfloat size;
public:
	Rect(GLfloat x, GLfloat y);

	GLvoid draw_rect();
};

Rect rect1(-0.5f, 0.5f);
Rect rect2(0.5f, 0.5f);
Rect rect3(0.5f, -0.5f);
Rect rect4(-0.5f, -0.5f);

void main(int argc, char** argv)
{
	//윈도우 생성
	glutInit(&argc, argv);											//glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);					//디스플레이 모드 설정
	glutInitWindowPosition(100, 100);								//윈도우 위치 설정
	glutInitWindowSize(500, 500);									//윈도우 크기 지정
	glutCreateWindow("Example1");									//윈도우 생성 (윈도우 이름)

	//GLEW 초기화
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)										//glew 초기화
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	glutDisplayFunc(drawScene);										//출력 함수의 지정
	glutReshapeFunc(Reshape);										//다시 그리기 함수의 지정
	glutMouseFunc(Mouse);
	glutMainLoop();													//이벤트 처리 시작
}

GLvoid drawScene()													//--- 콜백 함수 : 그리기 콜백 함수
{
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);							//바탕색을 'blue'로 지정
	glClear(GL_COLOR_BUFFER_BIT);									//설정된 색으로 전체를 칠하기
	//그리기 부분 구현
	//--- 그리기 관련 부분이 여기에 포함된다.
	rect1.draw_rect();
	rect2.draw_rect();
	rect3.draw_rect();
	rect4.draw_rect();
	glutSwapBuffers();												//화면에 출력하기
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

Rect::Rect(GLfloat x, GLfloat y) : r(distribution(rd)), g(distribution(rd)), b(distribution(rd)), center_x(x), center_y(y), size(0.5f) {}

GLvoid Rect::draw_rect() {
	glColor3f(r, g, b);
	glRectf(center_x - size, center_y - size, center_x + size, center_y + size);
}

GLvoid Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		check_mouse_pressed_in_rect1(x, y);
		check_mouse_pressed_in_rect2(x, y);
		check_mouse_pressed_in_rect3(x, y);
		check_mouse_pressed_in_rect4(x, y);
		check_mouse_pressed_in_BG(x, y);
	}
}

bool check_mouse_pressed_in_rect1(int x, int y) {
	return true;
}
bool check_mouse_pressed_in_rect2(int x, int y) {
	return true;
}
bool check_mouse_pressed_in_rect3(int x, int y) {
	return true;
}
bool check_mouse_pressed_in_rect4(int x, int y) {
	return true;
}
bool check_mouse_pressed_in_BG(int x, int y) {
	return true;
}