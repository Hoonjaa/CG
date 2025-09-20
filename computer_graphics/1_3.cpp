#include <iostream>
#include <random>
#include <vector>

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Motion(int x, int y);
GLvoid Keyboard(unsigned char key, int x, int y);

GLvoid Win_to_GL_mouse(int x, int y, GLfloat& gl_x, GLfloat& gl_y);

std::random_device rd;
std::uniform_real_distribution<float> distribution_color(0.0f, 1.0f);
std::uniform_real_distribution<float> distribution_coordinate(-0.9f, 0.9f);

class Rect {
	GLclampf r, g, b;
	GLfloat center_x, center_y;
	GLfloat size_w, size_h;

	bool dragging = false;
	GLfloat offset_x = 0.0f, offset_y = 0.0f;
public:
	Rect(GLfloat x, GLfloat y);
	Rect(GLfloat x, GLfloat y, GLfloat w, GLfloat h);
	//그리기 함수
	GLvoid draw_rect();
	GLvoid change_color();
	//마우스
	//마우스가 안에 있는지 체크
	bool mouse_check_in_rect(GLfloat x, GLfloat y);
	//마우스 드래그 함수
	GLvoid start_drag(GLfloat mouse_x, GLfloat mouse_y);
	GLvoid stop_drag() { dragging = false; }
	GLvoid drag_move(GLfloat mouse_x, GLfloat mouse_y);
	bool is_dragging() const { return dragging; }
	//사각형 나누기 함수
	GLvoid slice_rect();
	//사각형 충돌 함수
	bool collide_check(const Rect& other) const;
	//사각형 병합 함수
	GLvoid merge_rect(Rect& other);
};

int WindowWidth = 500, WindowHeight = 500;
std::vector<Rect> rects;
bool left_button = false;

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
	glutMotionFunc(Motion);
	glutKeyboardFunc(Keyboard);
	glutMainLoop();													//이벤트 처리 시작
}

GLvoid drawScene()													//--- 콜백 함수 : 그리기 콜백 함수
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);									//바탕색을 'blue'로 지정
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
		left_button = true;
		for (auto& rect : rects) {
			if (rect.mouse_check_in_rect(gl_x, gl_y)) {
				rect.start_drag(gl_x, gl_y);
				break;
			}
			else {
				rect.stop_drag();
			}
		}
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		left_button = false;
		for (auto& rect : rects) {
			if (rect.is_dragging()) {
				for (int i = 0; i < rects.size(); i++) {
					if (&rect != &rects[i] && rect.collide_check(rects[i])) {
						rect.merge_rect(rects[i]);
						rects.erase(rects.begin() + i);
						break;
					}
				}
			}
		}
		for (auto& rect : rects) rect.stop_drag();
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		for (auto& rect : rects) {
			if (rect.mouse_check_in_rect(gl_x, gl_y)) {
				rect.slice_rect();
			}
		}
	}
	glutPostRedisplay();
}

GLvoid Motion(int x, int y) {
	GLfloat gl_x, gl_y;
	Win_to_GL_mouse(x, y, gl_x, gl_y);
	for (auto& rect : rects) {
		if (rect.is_dragging()) {
			rect.drag_move(gl_x, gl_y);
		}
	}
	glutPostRedisplay();
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'a':
		rects.emplace_back(distribution_coordinate(rd), distribution_coordinate(rd));
		break;
	}
	glutPostRedisplay();
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

GLvoid Win_to_GL_mouse(int x, int y, GLfloat& gl_x, GLfloat& gl_y) {
	gl_x = (x / (float)WindowWidth) * 2.0f - 1.0f;
	gl_y = 1.0f - (y / (float)WindowHeight) * 2.0f;
}

GLvoid Rect::start_drag(GLfloat mouse_x, GLfloat mouse_y) {
	dragging = true;
	offset_x = mouse_x - center_x;
	offset_y = mouse_y - center_y;
}

GLvoid Rect::drag_move(GLfloat mouse_x, GLfloat mouse_y) {
	if (dragging) {
		center_x = mouse_x - offset_x;
		center_y = mouse_y - offset_y;
	}
}

GLvoid Rect::slice_rect() {
	//세로로 자르기
	if (size_h <= size_w) {
		center_x = center_x - (size_w / 2);
		size_w = (size_w / 2);
		rects.emplace_back(center_x + (2 * size_w), center_y, size_w, size_h);
	}
	//가로로 자르기
	else if (size_w < size_h) {
		center_y = center_y - (size_h / 2);
		size_h = (size_h / 2);
		rects.emplace_back(center_x, center_y + (2 * size_h), size_w, size_h);
	}
}

bool Rect::collide_check(const Rect& other) const {
	GLfloat leftA = center_x - size_w;
	GLfloat rightA = center_x + size_w;
	GLfloat topA = center_y + size_h;
	GLfloat bottomA = center_y - size_h;

	GLfloat leftB = other.center_x - other.size_w;
	GLfloat rightB = other.center_x + other.size_w;
	GLfloat topB = other.center_y + other.size_h;
	GLfloat bottomB = other.center_y - other.size_h;

	if (leftA > rightB || rightA < leftB) return false;
	if (topA < bottomB || bottomA > topB) return false;
	return true;
}

GLvoid Rect::merge_rect(Rect& other) {
	GLfloat leftA = center_x - size_w;
	GLfloat rightA = center_x + size_w;
	GLfloat topA = center_y + size_h;
	GLfloat bottomA = center_y - size_h;

	GLfloat leftB = other.center_x - other.size_w;
	GLfloat rightB = other.center_x + other.size_w;
	GLfloat topB = other.center_y + other.size_h;
	GLfloat bottomB = other.center_y - other.size_h;

	GLfloat left, right, top, bottom;
	left = (leftA <= leftB) ? leftA : leftB;
	right = (rightB <= rightA) ? rightA : rightB;
	top = (topB <= topA) ? topA : topB;
	bottom = (bottomA <= bottomB) ? bottomA : bottomB;

	size_w = (right - left) / 2; size_h = (top - bottom) / 2;
	center_x = right - size_w; center_y = top - size_h;
	r = distribution_color(rd); g = distribution_color(rd); b = distribution_color(rd);
}