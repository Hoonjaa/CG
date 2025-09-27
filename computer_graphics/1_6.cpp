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
	GLint ani_type = 0;
public:
	Rect(GLfloat x, GLfloat y);
	Rect(GLfloat x, GLfloat y, GLfloat w, GLfloat h);
	Rect(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLfloat r, GLfloat g, GLfloat b);
	//±×¸®±â ÇÔ¼ö
	GLvoid draw_rect();
	GLvoid change_color();
	//¸¶¿ì½º
	//¸¶¿ì½º°¡ ¾È¿¡ ÀÖ´ÂÁö Ã¼Å©
	bool mouse_check_in_rect(GLfloat x, GLfloat y);
	//¾Ö´Ï¸ŞÀÌ¼Ç
	GLvoid animation_on() { animation = true; }
	bool animation_check() { return animation; }
	//¼­ºê »ç°¢Çü »ı¼º
	GLvoid create_sub_rects();
};

int WindowWidth = 500, WindowHeight = 500;
std::vector<Rect> rects;
<<<<<<< HEAD
std::vector<Rect> first_sub_rects;
std::vector<Rect> second_sub_rects;
std::vector<Rect> third_sub_rects;
bool animation1 = false, animation2 = false, animation3 = false, animation4 = false;
=======
>>>>>>> parent of da20fc9 ([25.09.27] Update 1-6 // ì• ë‹ˆë©”ì´ì…˜ ìƒíƒœê°’ ìƒì„±)

void main(int argc, char** argv)
{
	//À©µµ¿ì »ı¼º
	glutInit(&argc, argv);											//glut ÃÊ±âÈ­
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);					//µğ½ºÇÃ·¹ÀÌ ¸ğµå ¼³Á¤
	glutInitWindowPosition(100, 100);								//À©µµ¿ì À§Ä¡ ¼³Á¤
	glutInitWindowSize(WindowWidth, WindowHeight);					//À©µµ¿ì Å©±â ÁöÁ¤
	glutCreateWindow("Example1");									//À©µµ¿ì »ı¼º (À©µµ¿ì ÀÌ¸§)

	//GLEW ÃÊ±âÈ­
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)										//glew ÃÊ±âÈ­
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	glutDisplayFunc(drawScene);										//Ãâ·Â ÇÔ¼öÀÇ ÁöÁ¤
	glutReshapeFunc(Reshape);										//´Ù½Ã ±×¸®±â ÇÔ¼öÀÇ ÁöÁ¤
	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard);
	for (int i = 0; i < 5; i++) {
		rects.emplace_back(distribution_coordinate(rd), distribution_coordinate(rd), distribution_size(rd), distribution_size(rd));
	}
	glutMainLoop();													//ÀÌº¥Æ® Ã³¸® ½ÃÀÛ
}

GLvoid drawScene()													//--- Äİ¹é ÇÔ¼ö : ±×¸®±â Äİ¹é ÇÔ¼ö
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);									//¼³Á¤µÈ »öÀ¸·Î ÀüÃ¼¸¦ Ä¥ÇÏ±â
	//±×¸®±â ºÎºĞ ±¸Çö
	//--- ±×¸®±â °ü·Ã ºÎºĞÀÌ ¿©±â¿¡ Æ÷ÇÔµÈ´Ù.
	for (auto& rect : rects) {
		rect.draw_rect();
	}
	glutSwapBuffers();												//È­¸é¿¡ Ãâ·ÂÇÏ±â
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

GLvoid TimerFunction(int value)
{
	for(auto& rect : rects) {
		if (rect.animation_check()) {
			rect.create_sub_rects();
			if (animation1) {  }
		}
	}
	glutPostRedisplay();
	glutTimerFunc(16, TimerFunction, 1);
}

GLvoid Rect::create_sub_rects() {
	if (first_sub_rects.empty()) {
		first_sub_rects.emplace_back(center_x - size_w / 2, center_y + size_h / 2, size_w / 2, size_h / 2, 0.0f, 1.0f, 0.0f);
		first_sub_rects.emplace_back(center_x + size_w / 2, center_y + size_h / 2, size_w / 2, size_h / 2, 0.0f, 1.0f, 0.0f);
		first_sub_rects.emplace_back(center_x + size_w / 2, center_y - size_h / 2, size_w / 2, size_h / 2, 0.0f, 1.0f, 0.0f);
		first_sub_rects.emplace_back(center_x - size_w / 2, center_y - size_h / 2, size_w / 2, size_h / 2, 0.0f, 1.0f, 0.0f);
	}
	else if (second_sub_rects.empty()) {
		second_sub_rects.emplace_back(center_x - size_w / 2, center_y + size_h / 2, size_w / 2, size_h / 2, 0.0f, 1.0f, 0.0f);
		second_sub_rects.emplace_back(center_x + size_w / 2, center_y + size_h / 2, size_w / 2, size_h / 2, 0.0f, 1.0f, 0.0f);
		second_sub_rects.emplace_back(center_x + size_w / 2, center_y - size_h / 2, size_w / 2, size_h / 2, 0.0f, 1.0f, 0.0f);
		second_sub_rects.emplace_back(center_x - size_w / 2, center_y - size_h / 2, size_w / 2, size_h / 2, 0.0f, 1.0f, 0.0f);
	}
	else if (third_sub_rects.empty()) {
		third_sub_rects.emplace_back(center_x - size_w / 2, center_y + size_h / 2, size_w / 2, size_h / 2, 0.0f, 1.0f, 0.0f);
		third_sub_rects.emplace_back(center_x + size_w / 2, center_y + size_h / 2, size_w / 2, size_h / 2, 0.0f, 1.0f, 0.0f);
		third_sub_rects.emplace_back(center_x + size_w / 2, center_y - size_h / 2, size_w / 2, size_h / 2, 0.0f, 1.0f, 0.0f);
		third_sub_rects.emplace_back(center_x - size_w / 2, center_y - size_h / 2, size_w / 2, size_h / 2, 0.0f, 1.0f, 0.0f);
	}
}