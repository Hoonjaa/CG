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

GLvoid start_animation();
GLvoid all_stop();
GLvoid Win_to_GL_mouse(int x, int y, GLfloat& gl_x, GLfloat& gl_y);

std::random_device rd;
std::uniform_real_distribution<float> distribution_color(0.0f, 1.0f);

class Rect {
	GLclampf r, g, b;
	GLfloat center_x, center_y;
	GLfloat size_w, size_h;
	GLfloat old_x, old_y;

	GLfloat begin_x, begin_y;
	GLfloat begin_w = 0.1f, begin_h = 0.1f;

	GLint move_type = 0;										//0 : 정지, 1 : 대각선, 2 : 지그재그
	bool is_change_color = false;
	bool is_change_size = false;
	GLfloat dis_x = 0.02f, dis_y = 0.02f;
	GLfloat add_size = 0.01f;
public:
	Rect(GLfloat x, GLfloat y);
	Rect(GLfloat x, GLfloat y, GLfloat w, GLfloat h);
	//그리기 함수
	GLvoid draw_rect();
	GLvoid change_color();
	GLvoid change_size();
	//애니메이션
	GLvoid set_animation(GLint type) { move_type = type; }
	GLvoid move_diagonal();
	GLvoid move_zigzag();
	GLvoid set_old_coord() { old_x = center_x; old_y = center_y; }
	GLint get_move_type() const { return move_type; }
	GLvoid color_mode_on() { is_change_color = true; }
	GLvoid color_mode_off() { is_change_color = false; }
	GLvoid size_mode_on() { is_change_size = true; }
	GLvoid size_mode_off() { is_change_size = false; }
	bool get_color_state() const { return is_change_color; }
	bool get_size_state() const { return is_change_size; }
	GLvoid stop();
	//
	GLvoid goto_begin();
};

int WindowWidth = 500, WindowHeight = 500;
std::vector<Rect> rects;
bool Timer = false;
bool ani1 = false, ani2 = false, ani3 = false, ani4 = false, ani5 = false;

void main(int argc, char** argv)
{
	//윈도우 생성
	glutInit(&argc, argv);											//glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);					//디스플레이 모드 설정
	glutInitWindowPosition(100, 100);								//윈도우 위치 설정
	glutInitWindowSize(WindowWidth, WindowHeight);					//윈도우 크기 지정
	glutCreateWindow("Example1-4");									//윈도우 생성 (윈도우 이름)

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
	glutMainLoop();													//이벤트 처리 시작
}

GLvoid drawScene()													//--- 콜백 함수 : 그리기 콜백 함수
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
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
		rects.emplace_back(gl_x, gl_y);
	}
	glutPostRedisplay();
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case '1':
		ani1 = !ani1;
		ani2 = false;
		if (ani1) {
			for (auto& rect : rects) {
				rect.set_animation(1);
			}
			if (!Timer) {
				start_animation();
			}
		}
		else {
			for (auto& rect : rects) {
				rect.set_animation(0);
			}
		}
		break;
	case '2':
		ani2 = !ani2;
		ani1 = false;
		if (ani2) {
			for (auto& rect : rects) {
				rect.set_old_coord();
				rect.set_animation(2);
			}
			if (!Timer) {
				start_animation();
			}
		}
		else {
			for (auto& rect : rects) {
				rect.set_animation(0);
			}
		}
		break;
	case '3':
		ani3 = !ani3;
		if (!Timer) {
			start_animation();
		}
		if (ani3) {
			for (auto& rect : rects) {
				rect.size_mode_on();
			}
		}
		else {
			for (auto& rect : rects) {
				rect.size_mode_off();
			}
		}
		break;
	case '4':
		ani4 = !ani4;
		if (!Timer) {
			start_animation();
		}
		if (ani4) {
			for (auto& rect : rects) {
				rect.color_mode_on();
			}
		}
		else {
			for (auto& rect : rects) {
				rect.color_mode_off();
			}
		}
		break;
	case '5':
		ani5 = !ani5;
		if (ani5) {
			for (int i = 0; i < rects.size(); i++) {
				if (i % 4 == 0) rects[i].set_animation(1);
				if (i % 4 == 1) rects[i].set_animation(2);
				if (i % 4 == 2) rects[i].size_mode_on();
				if (i % 4 == 3) rects[i].color_mode_on();
			}
			if (!Timer) {
				start_animation();
			}
		}
		else {
			all_stop();
		}
		break;
	case 's':
		all_stop();
		break;
	case 'm':
		for (auto& rect : rects) {
			rect.goto_begin();
		}
		break;
	case 'r':
		all_stop();
		rects.clear();
		break;
	case 'q':
		glutLeaveMainLoop();
		break;
	}
	glutPostRedisplay();
}

GLvoid Win_to_GL_mouse(int x, int y, GLfloat& gl_x, GLfloat& gl_y) {
	gl_x = (x / (float)WindowWidth) * 2.0f - 1.0f;
	gl_y = 1.0f - (y / (float)WindowHeight) * 2.0f;
}

Rect::Rect(GLfloat x, GLfloat y) : r(distribution_color(rd)), g(distribution_color(rd)), b(distribution_color(rd)), center_x(x), center_y(y)
, size_w(0.1f), size_h(0.1f), old_x(x), old_y(y), begin_x(x), begin_y(y) {}

GLvoid Rect::draw_rect() {
	glColor3f(r, g, b);
	glRectf(center_x - size_w, center_y - size_h, center_x + size_w, center_y + size_h);
}

GLvoid Rect::change_color() {
	r = distribution_color(rd); g = distribution_color(rd); b = distribution_color(rd);
}

GLvoid start_animation() {
	Timer = true;
	glutTimerFunc(16, TimerFunction, 1);
}

GLvoid Rect::move_diagonal() {
	center_x += dis_x; center_y += dis_y;
	if (center_x + size_w >= 1.0f || center_x - size_w <= -1.0f) {
		dis_x = -dis_x;
	}
	if (center_y + size_h >= 1.0f || center_y - size_h <= -1.0f) {
		dis_y = -dis_y;
	}
}

GLvoid Rect::move_zigzag() {
	center_x += dis_x; center_y += dis_y;
	if (center_x + size_w >= 1.0f || center_x - size_w <= -1.0f) {
		dis_x = -dis_x;
	}
	if (center_y + size_h >= 1.0f || center_y - size_h <= -1.0f) {
		dis_y = -dis_y;
	}
	else if (center_y + size_h >= old_y + size_h + 0.2f || center_y - size_h <= old_y - size_h - 0.2f) {
		dis_y = -dis_y;
	}
}

GLvoid TimerFunction(int value)
{
	for (auto& rect : rects) {
		if (rect.get_move_type() == 1) {
			rect.move_diagonal();
		}
		if (rect.get_move_type() == 2) {
			rect.move_zigzag();
		}
		if (rect.get_size_state()) {
			rect.change_size();
		}
		if (rect.get_color_state()) {
			rect.change_color();
		}
	}
	glutPostRedisplay();
	if (Timer) {
		glutTimerFunc(16, TimerFunction, 1);
	}
}

GLvoid Rect::change_size() {
	size_w += add_size; size_h += add_size;
	if (size_w <= 0.03f || size_h <= 0.03f) add_size = -add_size;
	else if (size_w >= 0.2f || size_h >= 0.2f) add_size = -add_size;
}

GLvoid Rect::stop() {
	is_change_color = false; is_change_size = false;
	move_type = 0;
}

GLvoid all_stop() {
	Timer = false;
	ani1 = false;
	ani2 = false;
	ani3 = false;
	ani4 = false;
	ani5 = false;
	for (auto& rect : rects) {
		rect.stop();
	}
}

GLvoid Rect::goto_begin() {
	center_x = begin_x; center_y = begin_y;
	size_w = begin_w; size_h = begin_h;
}