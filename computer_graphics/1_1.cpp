#include <iostream>
#include <random>

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);

std::random_device rd;
std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

GLclampf r = 0.0f, g = 0.0f, b = 0.0f;

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
	glutKeyboardFunc(Keyboard);
	glutMainLoop();													//이벤트 처리 시작
}

GLvoid drawScene()													//--- 콜백 함수 : 그리기 콜백 함수
{
	glClearColor(r, g, b, 1.0f);									//바탕색을 'blue'로 지정
	glClear(GL_COLOR_BUFFER_BIT);									//설정된 색으로 전체를 칠하기
	//그리기 부분 구현
	//--- 그리기 관련 부분이 여기에 포함된다.
	glutSwapBuffers();												//화면에 출력하기
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'c':
		r = 0.0f; g = 1.0f; b = 1.0f;
		break;
	case 'm':
		r = 1.0f; g = 0.0f; b = 1.0f;
		break;
	case 'y':
		r = 1.0f; g = 1.0f; b = 0.0f;
		break;
	case 'a':
		r = distribution(rd); g = distribution(rd); b = distribution(rd);
		break;
	case 'w':
		r = 1.0f; g = 1.0f; b = 1.0f;
		break;
	case 'k':
		r = 0.0f; g = 0.0f; b = 0.0f;
		break;
	case 't':
		break;
	case 's':
		break;
	case 'q':
		break;
	}
	glutPostRedisplay();
}