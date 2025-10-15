#include "pch.h"
#include "Axis.h"
#include "Triangle.h"

//--- 아래 5개 함수는 사용자 정의 함수임
void make_vertexShaders();
void make_fragmentShaders();
GLuint make_shaderProgram();
GLvoid drawScene();
GLvoid Reshape(int w, int h);
// 마우스 및 키보드	이벤트 처리 함수
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Win_to_GL_mouse(int x, int y, GLfloat& gl_x, GLfloat& gl_y);
// 마우스 이벤트 함수
GLvoid LButton(GLfloat x, GLfloat y);
GLvoid RButton(GLfloat x, GLfloat y);
// 타이머 함수
GLvoid TimerFunction(int value);
GLvoid start_Timer();

//--- 필요한 변수 선언
GLint width, height;
GLuint shaderProgramID;													//--- 세이더 프로그램 이름
GLuint vertexShader;													//--- 버텍스 세이더 객체
GLuint fragmentShader;													//--- 프래그먼트 세이더 객체

enum class SPACETYPE {
	SPACE_1,
	SPACE_2,
	SPACE_3,
	SPACE_4,
	END
};

GLint WindowWidth = 800, WindowHeight = 800;
Axis* gAxis = nullptr;
std::vector<Triangle*> triangles[(UINT)SPACETYPE::END];
GLint cDrawMode = (GLint)DRAWMODE::TRIANGLE;
bool Timer = false;

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
	gAxis = new Axis();
	triangles[(UINT)SPACETYPE::SPACE_1].push_back(new Triangle(0.5f, 0.5f, cDrawMode));
	triangles[(UINT)SPACETYPE::SPACE_2].push_back(new Triangle(-0.5f, 0.5f, cDrawMode));
	triangles[(UINT)SPACETYPE::SPACE_3].push_back(new Triangle(-0.5f, -0.5f, cDrawMode));
	triangles[(UINT)SPACETYPE::SPACE_4].push_back(new Triangle(0.5f, -0.5f, cDrawMode));
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
	vertexSource = filetobuf("vertex_9.glsl");
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
	fragmentSource = filetobuf("fragment_9.glsl");						// 프래그세이더 읽어오기
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

	if (gAxis) gAxis->draw();
	for (UINT i = 0; i < (UINT)SPACETYPE::END; ++i)
		for (size_t j = 0; j < triangles[i].size(); ++j)
			triangles[i][j]->draw();

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
		LButton(gl_x, gl_y);
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		RButton(gl_x, gl_y);
	}
	glutPostRedisplay();
}

GLvoid Win_to_GL_mouse(int x, int y, GLfloat& gl_x, GLfloat& gl_y) {
	gl_x = (x / (float)WindowWidth) * 2.0f - 1.0f;
	gl_y = 1.0f - (y / (float)WindowHeight) * 2.0f;
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'q':
		glutLeaveMainLoop();
		break;
	case 'c':
		for (UINT i = 0; i < (UINT)SPACETYPE::END; ++i)
			triangles[i].clear();
		break;
	case 'a':
		if (cDrawMode == (GLint)DRAWMODE::TRIANGLE)
			cDrawMode = (GLint)DRAWMODE::LINE_LOOP;
		else if (cDrawMode == (GLint)DRAWMODE::LINE_LOOP)
			cDrawMode = (GLint)DRAWMODE::TRIANGLE;
		break;
	case '1':
		start_Timer();
		for (UINT i = 0; i < (UINT)SPACETYPE::END; ++i)
			for (size_t j = 0; j < triangles[i].size(); ++j)
				triangles[i][j]->setMoveType((GLint)MOVE_TYPE::BOUNCING);
		break;
	case '2':
		start_Timer();
		for (UINT i = 0; i < (UINT)SPACETYPE::END; ++i)
			for (size_t j = 0; j < triangles[i].size(); ++j)
				triangles[i][j]->setMoveType((GLint)MOVE_TYPE::ZIGZAG_HORIZONTAL);
		break;
	case '3':
		start_Timer();
		for (UINT i = 0; i < (UINT)SPACETYPE::END; ++i)
			for (size_t j = 0; j < triangles[i].size(); ++j)
				triangles[i][j]->setMoveType((GLint)MOVE_TYPE::SPIRAL_SQUARE);
		break;
	case '4':
		start_Timer();
		for (UINT i = 0; i < (UINT)SPACETYPE::END; ++i)
			for (size_t j = 0; j < triangles[i].size(); ++j)
				triangles[i][j]->setMoveType((GLint)MOVE_TYPE::SPIRAL_CIRCLE);
		break;
	}
	glutPostRedisplay();
}

GLvoid LButton(GLfloat x, GLfloat y) {
	if (x >= 0.0f && y >= 0.0f) { // 1사분면
		triangles[(UINT)SPACETYPE::SPACE_1].clear();
		triangles[(UINT)SPACETYPE::SPACE_1].push_back(new Triangle(x, y, cDrawMode));
	}
	else if (x < 0.0f && y >= 0.0f) { // 2사분면
		triangles[(UINT)SPACETYPE::SPACE_2].clear();
		triangles[(UINT)SPACETYPE::SPACE_2].push_back(new Triangle(x, y, cDrawMode));
	}
	else if (x < 0.0f && y < 0.0f) { // 3사분면
		triangles[(UINT)SPACETYPE::SPACE_3].clear();
		triangles[(UINT)SPACETYPE::SPACE_3].push_back(new Triangle(x, y, cDrawMode));
	}
	else if (x >= 0.0f && y < 0.0f) { // 4사분면
		triangles[(UINT)SPACETYPE::SPACE_4].clear();
		triangles[(UINT)SPACETYPE::SPACE_4].push_back(new Triangle(x, y, cDrawMode));
	}
}

GLvoid RButton(GLfloat x, GLfloat y) {
	if (x >= 0.0f && y >= 0.0f && triangles[(UINT)SPACETYPE::SPACE_1].size() < 4) { // 1사분면
		triangles[(UINT)SPACETYPE::SPACE_1].push_back(new Triangle(x, y, cDrawMode));
	}
	else if (x < 0.0f && y >= 0.0f && triangles[(UINT)SPACETYPE::SPACE_2].size() < 4) { // 2사분면
		triangles[(UINT)SPACETYPE::SPACE_2].push_back(new Triangle(x, y, cDrawMode));
	}
	else if (x < 0.0f && y < 0.0f && triangles[(UINT)SPACETYPE::SPACE_3].size() < 4) { // 3사분면
		triangles[(UINT)SPACETYPE::SPACE_3].push_back(new Triangle(x, y, cDrawMode));
	}
	else if (x >= 0.0f && y < 0.0f && triangles[(UINT)SPACETYPE::SPACE_4].size() < 4) { // 4사분면
		triangles[(UINT)SPACETYPE::SPACE_4].push_back(new Triangle(x, y, cDrawMode));
	}
}

GLvoid TimerFunction(int value)
{
	for (UINT i = 0; i < (UINT)SPACETYPE::END; ++i)
		for (size_t j = 0; j < triangles[i].size(); ++j)
			triangles[i][j]->update();
	glutPostRedisplay();
	if (Timer) {
		glutTimerFunc(16, TimerFunction, 1);
	}
}

GLvoid start_Timer() {
	if (!Timer) {
		Timer = true;
		glutTimerFunc(16, TimerFunction, 1);
	}
}