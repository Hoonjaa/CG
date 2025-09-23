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
	Rect(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLfloat r, GLfloat g, GLfloat b);
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
	//플레이어
	GLvoid player_setting();
	//충돌 체크
	GLvoid crash_check();
	//위치 지정
	GLvoid set_coordinate(GLfloat x, GLfloat y);
};

int WindowWidth = 500, WindowHeight = 500;
std::vector<Rect> rects;
Rect player(0.0f, 0.0f, 0.1f, 0.1f, 0.0f, 0.0f, 0.0f);
bool player_on = false;

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
	for (int i = 0; i < 20; i++) {
		rects.emplace_back(distribution_coordinate(rd), distribution_coordinate(rd));
	}
	glutMainLoop();													//이벤트 처리 시작
}

GLvoid drawScene()													//--- 콜백 함수 : 그리기 콜백 함수
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);							//바탕색을 'blue'로 지정
	glClear(GL_COLOR_BUFFER_BIT);									//설정된 색으로 전체를 칠하기
	//그리기 부분 구현
	//--- 그리기 관련 부분이 여기에 포함된다.
	for (auto& rect : rects) {
		rect.draw_rect();
	}
	if(player_on)
		player.draw_rect();
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
		player_on = true;
		player.set_coordinate(gl_x, gl_y);
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		player_on = false;
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		
	}
	glutPostRedisplay();
}

GLvoid Motion(int x, int y) {
	GLfloat gl_x, gl_y;
	Win_to_GL_mouse(x, y, gl_x, gl_y);
	if (player_on) {
		player.set_coordinate(gl_x, gl_y);
		player.crash_check();
	}
	glutPostRedisplay();
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'r':
		rects.clear();
		for (int i = 0; i < 20; i++) {
			rects.emplace_back(distribution_coordinate(rd), distribution_coordinate(rd));
		}
		player.player_setting();
		break;
	}
	glutPostRedisplay();
}

Rect::Rect(GLfloat x, GLfloat y) : r(distribution_color(rd)), g(distribution_color(rd)), b(distribution_color(rd)), center_x(x), center_y(y), size_w(0.05f), size_h(0.05f) {}
Rect::Rect(GLfloat x, GLfloat y, GLfloat w, GLfloat h) : r(distribution_color(rd)), g(distribution_color(rd)), b(distribution_color(rd)), center_x(x), center_y(y), size_w(w), size_h(h) {}
Rect::Rect(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLfloat r, GLfloat g, GLfloat b) : r(r), g(g), b(b), center_x(x), center_y(y), size_w(w), size_h(h) {}

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

GLvoid Rect::player_setting() {
	r = 0.0f; g = 0.0f; b = 0.0f;
	size_w = 0.1f; size_h = 0.1f;
}

GLvoid Rect::crash_check() {
	for (int i = 0; i < rects.size(); i++) {
		if (rects[i].center_x >= center_x - size_w
			&& rects[i].center_x <= center_x + size_w
			&& rects[i].center_y >= center_y - size_h
			&& rects[i].center_y <= center_y + size_h) {
			rects.erase(rects.begin() + i);
			size_w += 0.02f; size_h += 0.02f;
		}
	}
}

GLvoid Rect::set_coordinate(GLfloat x, GLfloat y) {
	center_x = x; center_y = y;
}