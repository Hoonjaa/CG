#include <iostream>
#include <random>
#include <vector>

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid TimerFunction(int value);

GLvoid Win_to_GL_mouse(int x, int y, GLfloat& gl_x, GLfloat& gl_y);

std::random_device rd;
std::uniform_real_distribution<float> distribution_color(0.0f, 1.0f);
std::uniform_real_distribution<float> distribution_coordinate(-0.8f, 0.8f);
std::uniform_real_distribution<float> distribution_size(0.05f, 0.15f);

class Rect {
	GLclampf r, g, b;
	GLfloat center_x, center_y;
	GLfloat size_w, size_h;

	bool animation = false;
public:
	Rect(GLfloat x, GLfloat y);
	Rect(GLfloat x, GLfloat y, GLfloat w, GLfloat h);
	//그리기 함수
	GLvoid draw_rect();
	GLvoid change_color();
	//마우스
	//마우스가 안에 있는지 체크
	bool mouse_check_in_rect(GLfloat x, GLfloat y);
	//애니메이션
	GLvoid animation_on() { animation = true; }
	bool animation_check() { return animation; }
	GLvoid first_animation();
};

int WindowWidth = 500, WindowHeight = 500;
std::vector<Rect> rects;
bool animation1 = false, animation2 = false, animation3 = false, animation4 = false;

void main(int argc, char** argv)
{
	//윈도우 생성
	glutInit(&argc, argv);											//glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);					//디스플레이 모드 설정
	glutInitWindowPosition(100, 100);								//윈도우 위치 설정
	glutInitWindowSize(WindowWidth, WindowHeight);					//윈도우 크기 지정
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
	glutKeyboardFunc(Keyboard);
	for (int i = 0; i < 5; i++) {
		rects.emplace_back(distribution_coordinate(rd), distribution_coordinate(rd), distribution_size(rd), distribution_size(rd));
	}
	glutMainLoop();													//이벤트 처리 시작
}

GLvoid drawScene()													//--- 콜백 함수 : 그리기 콜백 함수
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);									//설정된 색으로 전체를 칠하기
	//그리기 부분 구현
	//--- 그리기 관련 부분이 여기에 포함된다.
	for (auto& rect : rects) {
		rect.draw_rect();
	}
	glutSwapBuffers();												//화면에 출력하기
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

GLvoid Mouse(int button, int state, int x, int y) {
	GLfloat gl_x, gl_y;
	Win_to_GL_mouse(x, y, gl_x, gl_y);
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		for(auto& rect : rects) {
			if (rect.mouse_check_in_rect(gl_x, gl_y)) {
				rect.animation_on();
				glutTimerFunc(16, TimerFunction, 1);
			}
		}
	}
	glutPostRedisplay();
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case '1':
		animation1 = false; animation2 = false; animation3 = false; animation4 = false;
		animation1 = true;
		break;
	case '2':
		animation1 = false; animation2 = false; animation3 = false; animation4 = false;
		animation2 = true;
		break;
	case '3':
		animation1 = false; animation2 = false; animation3 = false; animation4 = false;
		animation3 = true;
		break;
	case '4':
		animation1 = false; animation2 = false; animation3 = false; animation4 = false;
		animation4 = true;
		break;
	}
	glutPostRedisplay();
}

GLvoid Win_to_GL_mouse(int x, int y, GLfloat& gl_x, GLfloat& gl_y) {
	gl_x = (x / (float)WindowWidth) * 2.0f - 1.0f;
	gl_y = 1.0f - (y / (float)WindowHeight) * 2.0f;
}

Rect::Rect(GLfloat x, GLfloat y) : r(distribution_color(rd)), g(distribution_color(rd)), b(distribution_color(rd)), center_x(x), center_y(y), size_w(0.1f), size_h(0.1f) {}
Rect::Rect(GLfloat x, GLfloat y, GLfloat w, GLfloat h) : r(distribution_color(rd)), g(distribution_color(rd)), b(distribution_color(rd)), center_x(x), center_y(y), size_w(w), size_h(h) {}

GLvoid Rect::draw_rect() {
	glColor3f(r, g, b);
	glRectf(center_x - size_w, center_y - size_h, center_x + size_w, center_y + size_h);
}

GLvoid Rect::change_color() {
	r = distribution_color(rd); g = distribution_color(rd); b = distribution_color(rd);
}

bool Rect::mouse_check_in_rect(GLfloat x, GLfloat y) {
	return (x >= center_x - size_w && x <= center_x + size_w &&
		y >= center_y - size_h && y <= center_y + size_h);
}

GLvoid TimerFunction(int value)
{
	for(auto& rect : rects) {
		if (rect.animation_check()) {
			if (animation1) { rect.first_animation(); }
		}
	}
	glutPostRedisplay();
	glutTimerFunc(16, TimerFunction, 1);
}

GLvoid Rect::first_animation() {
	
}