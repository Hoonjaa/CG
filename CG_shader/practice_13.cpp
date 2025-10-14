#include "pch.h"
#include "Axis.h"
#include "TPentagon13.h"

//--- 아래 5개 함수는 사용자 정의 함수임
void make_vertexShaders();
void make_fragmentShaders();
GLuint make_shaderProgram();
GLvoid drawScene();
GLvoid Reshape(int w, int h);
// 키보드 이벤트 처리 함수
GLvoid Keyboard(unsigned char key, int x, int y);
// 마우스 이벤트 처리 함수
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Motion(int x, int y);
GLvoid Win_to_GL_mouse(int x, int y, GLfloat& gl_x, GLfloat& gl_y);
// 타이머 함수
GLvoid TimerFunction(int value);
GLvoid start_Timer();

//--- 필요한 변수 선언
GLint width, height;
GLuint shaderProgramID;													//--- 세이더 프로그램 이름
GLuint vertexShader;													//--- 버텍스 세이더 객체
GLuint fragmentShader;													//--- 프래그먼트 세이더 객체

GLint WindowWidth = 800, WindowHeight = 800;
std::vector<TPentagon13*> objects;
std::vector<TPentagon13*> move_objects;
GLint cDrawMode = (GLint)DRAWMODE::TRIANGLE;
bool Timer = true;
GLint selected_object = -1;
GLint new_object = -1;

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
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WindowWidth, WindowHeight);
	glutCreateWindow("Example1");
	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();
	//--- 세이더 읽어와서 세이더 프로그램 만들기: 사용자 정의함수 호출
	make_vertexShaders();												//--- 버텍스 세이더 만들기
	make_fragmentShaders();												//--- 프래그먼트 세이더 만들기
	shaderProgramID = make_shaderProgram();
	//--- 세이더 프로그램 만들기
	for (int i = 0; i < 3; i++) {
		objects.push_back(new TPentagon13(random_col(rd), random_col13(rd), 1.0f, 1, cDrawMode));
		objects.push_back(new TPentagon13(random_col(rd), random_col13(rd), 1.0f, 2, cDrawMode));
		objects.push_back(new TPentagon13(random_col(rd), random_col13(rd), 1.0f, 3, cDrawMode));
		objects.push_back(new TPentagon13(random_col(rd), random_col13(rd), 1.0f, 4, cDrawMode));
		objects.push_back(new TPentagon13(random_col(rd), random_col13(rd), 1.0f, 5, cDrawMode));
	}
	glutTimerFunc(40, TimerFunction, 1);
	glutDisplayFunc(drawScene);											//--- 출력 콜백 함수
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutMainLoop();
}

//--- 버텍스 세이더 객체 만들기
void make_vertexShaders()
{
	GLchar* vertexSource;
	//--- 버텍스 세이더 읽어 저장하고 컴파일 하기
	//--- filetobuf: 사용자정의 함수로 텍스트를 읽어서 문자열에 저장하는 함수
	vertexSource = filetobuf("vertex.glsl");
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
	fragmentSource = filetobuf("fragment.glsl");						// 프래그세이더 읽어오기
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

//--- 출력 콜백 함수
GLvoid drawScene()														//--- 콜백 함수: 그리기 콜백 함수
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(shaderProgramID);
	glPointSize(5.0);

	for (size_t i = 0; i < objects.size(); i++) {
		objects[i]->draw();
	}
	for (size_t i = 0; i < move_objects.size(); i++) {
		move_objects[i]->draw();
	}

	glutSwapBuffers();													// 화면에 출력하기
}
//--- 다시그리기 콜백 함수
GLvoid Reshape(int w, int h)											//--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'q':
		glutLeaveMainLoop();
		break;
	case 'c':
		objects.clear();
		move_objects.clear();
		for (int i = 0; i < 3; i++) {
			objects.push_back(new TPentagon13(random_col(rd), random_col13(rd), 1.0f, 1, cDrawMode));
			objects.push_back(new TPentagon13(random_col(rd), random_col13(rd), 1.0f, 2, cDrawMode));
			objects.push_back(new TPentagon13(random_col(rd), random_col13(rd), 1.0f, 3, cDrawMode));
			objects.push_back(new TPentagon13(random_col(rd), random_col13(rd), 1.0f, 4, cDrawMode));
			objects.push_back(new TPentagon13(random_col(rd), random_col13(rd), 1.0f, 5, cDrawMode));
		}
		break;
	}
	glutPostRedisplay();
}

GLvoid TimerFunction(int value)
{
	
	glutPostRedisplay();
	if (Timer) {
		glutTimerFunc(40, TimerFunction, 1);
	}
}

GLvoid start_Timer() {
	if (!Timer) {
		Timer = true;
		glutTimerFunc(16, TimerFunction, 1);
	}
}

GLvoid Motion(int x, int y) {
	GLfloat gl_x, gl_y;
	Win_to_GL_mouse(x, y, gl_x, gl_y);
	if (selected_object == -1) return;
	objects[selected_object]->setPos(vec3(gl_x, gl_y, 0.0f));
	objects[selected_object]->setDragVertex(gl_x, gl_y);
	glutPostRedisplay();
}

GLvoid Mouse(int button, int state, int x, int y) {
	GLfloat gl_x, gl_y;
	Win_to_GL_mouse(x, y, gl_x, gl_y);
	vec3 Opos;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		for (size_t i = 0; i < objects.size(); i++) {
			Opos = objects[i]->getPos();
			GLfloat dist = sqrt((gl_x - Opos.x) * (gl_x - Opos.x) + (gl_y - Opos.y) * (gl_y - Opos.y));
			if (dist < 0.1f) {
				selected_object = i;
				break;
			}
		}
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		for (size_t i = 0; i < objects.size(); i++) {
			Opos = objects[i]->getPos();
			GLfloat dist = sqrt((gl_x - Opos.x) * (gl_x - Opos.x) + (gl_y - Opos.y) * (gl_y - Opos.y));
			if (dist < 0.1f && selected_object != -1 && selected_object != i) {
				GLint new_type = (objects[selected_object]->getShapeType() + objects[i]->getShapeType());
				if (new_type > 5) new_type = 1;
				if (selected_object > i) {
					objects.erase(objects.begin() + selected_object);
					objects.erase(objects.begin() + i);
				}else{
					objects.erase(objects.begin() + i);
					objects.erase(objects.begin() + selected_object);
				}
				
				move_objects.push_back(new TPentagon13(gl_x, gl_y, 1.0f, new_type, cDrawMode));
				break;
			}
		}
		if (selected_object != -1) {
			selected_object = -1;
		}
	}
	glutPostRedisplay();
}

GLvoid Win_to_GL_mouse(int x, int y, GLfloat& gl_x, GLfloat& gl_y) {
	gl_x = (x / (float)WindowWidth) * 2.0f - 1.0f;
	gl_y = 1.0f - (y / (float)WindowHeight) * 2.0f;
}
