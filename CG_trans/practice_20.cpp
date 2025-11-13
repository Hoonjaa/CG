#include "pch.h"
#include "Coordinate_system.h"
#include "TreeNode.h"
#include "TankPart.h"
#include "Tank_low_body.h"
#include "Tank_middle_body.h"
#include "Tank_top_body.h"
#include "cannon.h"
#include "Tank_flag.h"

//--- 아래 5개 함수는 사용자 정의 함수임
void make_vertexShaders();
void make_fragmentShaders();
GLuint make_shaderProgram();
GLvoid drawScene();
GLvoid Reshape(int w, int h);
// 키보드 이벤트 처리 함수
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid SpecialKeyboard(int key, int x, int y);
// 마우스 이벤트 처리 함수
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Motion(int x, int y);
GLvoid Win_to_GL_mouse(int x, int y, GLfloat& gl_x, GLfloat& gl_y);
// 타이머 함수
GLvoid TimerFunction(int value);
// 뷰 및 투영 변환 행렬
GLvoid setViewPerspectiveMatrix();
// 변환 행렬 업데이트
GLvoid updateTransformMatrix();

//--- 필요한 변수 선언
GLuint shaderProgramID;													//--- 세이더 프로그램 이름
GLuint vertexShader;													//--- 버텍스 세이더 객체
GLuint fragmentShader;													//--- 프래그먼트 세이더 객체

GLint WindowWidth = 800, WindowHeight = 800;
bool Timer = false;
glm::mat4 Transform_matrix{ 1.0f };

Coordinate_system* coordinate_system = nullptr;

class Tank {
private:
	std::shared_ptr<TreeNode> root;
	std::shared_ptr<TankPart> lowBodyPart;
	std::shared_ptr<TankPart> middleBodyPart;
	std::shared_ptr<TankPart> topBody1Part;
	std::shared_ptr<TankPart> topBody2Part;
	std::shared_ptr<TankPart> cannon1Part;
	std::shared_ptr<TankPart> cannon2Part;
	std::shared_ptr<TankPart> flag1Part;
	std::shared_ptr<TankPart> flag2Part;

	glm::vec3 position{ 0.0f, 0.0f, 0.0f };

public:
	GLvoid setup(GLuint shader) {
		root = std::make_shared<TreeNode>();

		// 하부 몸체 생성
		auto lowBodyObj = std::make_shared<Tank_low_body>();
		lowBodyPart = std::make_shared<TankPart>(lowBodyObj, shader);
		root->addChild(lowBodyPart);

		// 중간 몸체 생성
		auto middleBodyObj = std::make_shared<Tank_middle_body>();
		middleBodyPart = std::make_shared<TankPart>(middleBodyObj, shader);
		middleBodyPart->translate(glm::vec3(0.0f, 0.7f, 0.0f)); // 중간 몸체를 하부 몸체 위로 올리기
		lowBodyPart->addChild(middleBodyPart);

		// 상부 몸체1 생성
		auto topBodyObj = std::make_shared<Tank_top_body>();
		topBody1Part = std::make_shared<TankPart>(topBodyObj, shader);
		topBody1Part->translate(glm::vec3(1.0f, 0.6f, 0.0f)); // 상부 몸체를 중간 몸체 위로 올리기
		middleBodyPart->addChild(topBody1Part);

		//상부 몸체2 생성
		auto topBodyObj2 = std::make_shared<Tank_top_body>();
		topBody2Part = std::make_shared<TankPart>(topBodyObj2, shader);
		topBody2Part->translate(glm::vec3(-1.0f, 0.6f, 0.0f)); // 상부 몸체를 중간 몸체 위로 올리기
		middleBodyPart->addChild(topBody2Part);

		// 포신1 생성
		auto cannonObj1 = std::make_shared<cannon>();
		cannon1Part = std::make_shared<TankPart>(cannonObj1, shader);
		cannon1Part->translate(glm::vec3(0.0f, 0.0f, 1.8f));
		topBody1Part->addChild(cannon1Part);

		// 포신2 생성
		auto cannonObj2 = std::make_shared<cannon>();
		cannon2Part = std::make_shared<TankPart>(cannonObj2, shader);
		cannon2Part->translate(glm::vec3(0.0f, 0.0f, 1.8f));	
		topBody2Part->addChild(cannon2Part);

		// 깃발1 생성
		auto flagObj1 = std::make_shared<Tank_flag>();
		flag1Part = std::make_shared<TankPart>(flagObj1, shader);
		flag1Part->translate(glm::vec3(0.0f, 0.8f, -0.2f));
		topBody1Part->addChild(flag1Part);

		// 깃발2 생성
		auto flagObj2 = std::make_shared<Tank_flag>();
		flag2Part = std::make_shared<TankPart>(flagObj2, shader);
		flag2Part->translate(glm::vec3(0.0f, 0.8f, -0.2f));
		topBody2Part->addChild(flag2Part);
	}

	GLvoid MoveTank(const glm::vec3& distance) {
		position += distance;
		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, position);
		root->setTransform(transform);
	}

	GLvoid render(const glm::mat4& viewProjectionMatrix) {
		if (root) {
			root->render(viewProjectionMatrix);
		}
	}
};

Tank* tank = nullptr;

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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
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
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	coordinate_system = new Coordinate_system();
	tank = new Tank();
	tank->setup(shaderProgramID);
	setViewPerspectiveMatrix();
	glutDisplayFunc(drawScene);											//--- 출력 콜백 함수
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKeyboard);
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramID);
	glPointSize(5.0);

	//if (coordinate_system) coordinate_system->draw(shaderProgramID,Transform_matrix);
	if (tank) tank->render(Transform_matrix);

	glutSwapBuffers();													// 화면에 출력하기
}
//--- 다시그리기 콜백 함수
GLvoid Reshape(int w, int h)											//--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}

GLvoid Motion(int x, int y) {
	GLfloat gl_x, gl_y;
	Win_to_GL_mouse(x, y, gl_x, gl_y);

	glutPostRedisplay();
}

GLvoid Mouse(int button, int state, int x, int y) {
	GLfloat gl_x, gl_y;
	Win_to_GL_mouse(x, y, gl_x, gl_y);
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {

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
	case 'h':
		if (glIsEnabled(GL_DEPTH_TEST))
			glDisable(GL_DEPTH_TEST);
		else
			glEnable(GL_DEPTH_TEST);
		break;
	case 'u':
		if (glIsEnabled(GL_CULL_FACE))
			glDisable(GL_CULL_FACE);
		else
			glEnable(GL_CULL_FACE);
		break;
	}
	glutPostRedisplay();
}

GLvoid SpecialKeyboard(int key, int x, int y)
{
	switch (key) {
		case GLUT_KEY_UP:
			tank->MoveTank(glm::vec3(0.0f, 0.0f, 0.1f));
			break;
		case GLUT_KEY_DOWN:
			tank->MoveTank(glm::vec3(0.0f, 0.0f, -0.1f));
			break;
		case GLUT_KEY_LEFT:
			tank->MoveTank(glm::vec3(-0.1f, 0.0f, 0.0f));
			break;
		case GLUT_KEY_RIGHT:
			tank->MoveTank(glm::vec3(0.1f, 0.0f, 0.0f));
			break;
	}
	glutPostRedisplay();
}

GLvoid TimerFunction(int value)
{

	glutPostRedisplay();
	if (Timer) {
		glutTimerFunc(16, TimerFunction, 1);
	}
}

GLvoid setViewPerspectiveMatrix() {
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 2.0f, 10.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

	Transform_matrix = projection * view;
	updateTransformMatrix();
}

GLvoid updateTransformMatrix() {
	unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "Transform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Transform_matrix));
}