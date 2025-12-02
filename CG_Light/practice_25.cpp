#include "pch.h"
#include "Cube.h"
#include "Orbit.h"
#include "Light_cube.h"
#include "Square_horn.h"

//--- 아래 5개 함수는 사용자 정의 함수임
void make_vertexShaders();
void make_fragmentShaders();
GLuint make_shaderProgram();
GLvoid drawScene();
GLvoid Reshape(int w, int h);
// 키보드 이벤트 처리 함수
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid KeyboardUp(unsigned char key, int x, int y);
GLvoid SpecialKeyboard(int key, int x, int y);
// 마우스 이벤트 처리 함수
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Motion(int x, int y);
GLvoid PassiveMotion(int x, int y); // 추가: 마우스 버튼을 누르지 않은 상태에서의 움직임
GLvoid Win_to_GL_mouse(int x, int y, GLfloat& gl_x, GLfloat& gl_y);
// 타이머 함수
GLvoid TimerFunction(int value);

// --------------여기부터 필요한 함수들 선언하는 구역-------------
// 뷰 및 투영 변환 행렬
GLvoid setViewPerspectiveMatrix();
glm::mat4 getViewPerspectiveMatrix();
glm::mat4 getThirdPersonViewMatrix();
// 변환 행렬 업데이트
GLvoid updateTransformMatrix();



//--- 필요한 변수 선언
GLuint shaderProgramID;													//--- 세이더 프로그램 이름
GLuint vertexShader;													//--- 버텍스 세이더 객체
GLuint fragmentShader;													//--- 프래그먼트 세이더 객체

// --------------여기부터 필요한 변수들 선언하는 구역-------------

GLint WindowWidth = 800, WindowHeight = 800;
bool Timer = true; // 타이머 사용중
glm::mat4 Transform_matrix{ 1.0f };

GLfloat LightColor = 1.0f;
glm::vec3 LightPos = glm::vec3(0.0f, 0.0f, 2.0f);

bool light_on = true;

bool object_rotate = false;
bool light_orbit_rotate = false;
GLfloat light_orbit_angle = 0.0f;
bool show_cube = true;  // true면 cube 표시, false면 사각뿔 표시

//Cube* cube = nullptr; 예시임 포인터로 객체 선언
// 포인터로 하는 이유는 셰이더가 만들어지는 등 기본 세팅 코드가 먼저 작동해야 객체 생성가능
// 따라서 main 함수 안에서 new로 객체 생성
Cube* cube = nullptr;
Square_horn* square_horn = nullptr;
Orbit* orbit_1 = nullptr;
Light_cube* light_cube = nullptr;

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


	// --------------기본 객체 생성 시 여기서 작업-------------
	cube = new Cube();
	square_horn = new Square_horn();
	orbit_1 = new Orbit(2.0f);
	light_cube = new Light_cube();
	
	// Light_cube의 초기 위치를 orbit 위치로 설정
	glm::vec3 initialOrbitPos = orbit_1->getPositionOnOrbit(light_orbit_angle);
	LightPos = initialOrbitPos;
	light_cube->setPosition(initialOrbitPos);

	glutTimerFunc(16, TimerFunction, 1);
	setViewPerspectiveMatrix();
	glutDisplayFunc(drawScene);											//--- 출력 콜백 함수
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutKeyboardUpFunc(KeyboardUp);
	glutSpecialFunc(SpecialKeyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutPassiveMotionFunc(PassiveMotion); // 추가: 마우스 버튼을 누르지 않은 상태에서의 움직임 감지
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

	// 조명과 Light_cube의 위치를 항상 orbit 위치에 맞춤
	glm::vec3 orbitPos = orbit_1->getPositionOnOrbit(light_orbit_angle);
	LightPos = orbitPos;
	light_cube->setPosition(orbitPos);

	// 이 model 같은 경우엔 프래그먼트 셰이더에서 조명 계산할 때 필요함
	// 뷰, 투영 제외한 월드 좌표계만 변환 행렬한거임
	// 일단은 단위 행렬로 설정해야 해서 이렇게 둘거임
	// 추후에 객체의 draw함수에서 별도로 설정해줄 것
	// 설정이 끝나면 아래 3줄 코드는 삭제해도 무방함.
	glm::mat4 model = glm::mat4(1.0f);
	unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));


	// 조명 관련 uniform 변수들 설정
	unsigned int lightPosLocation = glGetUniformLocation(shaderProgramID, "lightPos"); //--- lightPos 값 전달: (0.0, 0.0, 5.0);
	glUniform3f(lightPosLocation, LightPos[0], LightPos[1], LightPos[2]);
	unsigned int lightColorLocation = glGetUniformLocation(shaderProgramID, "lightColor"); //--- lightColor 값 전달: (1.0, 1.0, 1.0) 백색
	glUniform3f(lightColorLocation, LightColor, LightColor, LightColor);

	// ----------------여기서 객체 그리기--------------------
	// cube->draw(shaderProgramID, Transform_matrix); 이런식
	// cube 내부에서 유니폼 변수를 바꿔주기 위해 shaderProgramID 전달
	// cube 내부에서 최종 변환 행렬을 설정해야하기 때문에 Transform_matrix 전달
	// 현재 Transform_matrix는 뷰 및 투영 변환 행렬임

	// 결과적으론 cube->draw(shaderProgramID, Transform_matrix); 함수 하나 만으로 애니메이션까지 다 처리 가능하게
	orbit_1->draw(shaderProgramID, glm::mat4(1.0f), Transform_matrix);
	
	// n 키로 cube와 사각뿔 전환
	if (show_cube) {
		cube->draw(shaderProgramID, glm::mat4(1.0f), Transform_matrix);
	}
	else {
		square_horn->draw(shaderProgramID, glm::mat4(1.0f), Transform_matrix);
	}
	
	light_cube->draw(shaderProgramID, glm::mat4(1.0f), Transform_matrix);
	

	glutSwapBuffers();													// 화면에 출력하기
}
//--- 다시그리기 콜백 함수
GLvoid Reshape(int w, int h)											//--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}

GLvoid Motion(int x, int y) {

	glutPostRedisplay();
}

GLvoid PassiveMotion(int x, int y) {
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
	case 'm':
		if (light_on) {
			unsigned int turnLocation = glGetUniformLocation(shaderProgramID, "turn_off");
			glUniform1i(turnLocation, 1);
			light_on = false;
		}
		else {
			unsigned int turnLocation = glGetUniformLocation(shaderProgramID, "turn_off");
			glUniform1i(turnLocation, 0);
			light_on = true;
		}
		break;
	case 'y':
		object_rotate = !object_rotate;
		break;
	case 'r':
		light_orbit_rotate = !light_orbit_rotate;
		break;
	case 'z':
		orbit_1->radius += 0.5f;
		orbit_1->update();
		break;
	case 'Z':
		orbit_1->radius -= 0.5f;
		if (orbit_1->radius < 0.0f) {
			orbit_1->radius = 0.0f;
		}
		orbit_1->update();
		break;
	case 'n':
		show_cube = !show_cube;
		break;
	}
	glutPostRedisplay();
}

GLvoid KeyboardUp(unsigned char key, int x, int y) {
	switch (key) {

	}
	glutPostRedisplay();
}

GLvoid SpecialKeyboard(int key, int x, int y)
{
	switch (key) {
	
	}
	glutPostRedisplay();
}

GLvoid TimerFunction(int value)
{
	if (object_rotate) {
		cube->y_angle += 0.5f;
		square_horn->y_angle += 0.5f;
	}
	if (light_orbit_rotate) {
		light_orbit_angle += 0.02f;
		// 각도가 2π를 넘으면 0으로 리셋
		if (light_orbit_angle >= 2.0f * 3.14159265359f) {
			light_orbit_angle = 0.0f;
		}
	}
	glutPostRedisplay();
	if (Timer) {
		glutTimerFunc(16, TimerFunction, 1);
	}
}

GLvoid setViewPerspectiveMatrix() {
	glm::vec3 eye = glm::vec3(-2.0f, 2.0f, 10.0f);
	unsigned int viewPosLocation = glGetUniformLocation(shaderProgramID, "viewPos");
	glUniform3f(viewPosLocation, eye.x, eye.y, eye.z);

	glm::mat4 view = glm::lookAt(eye,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

	Transform_matrix = projection * view;
	updateTransformMatrix();
}

glm::mat4 getViewPerspectiveMatrix() {
	glm::vec3 eye = glm::vec3(-2.0f, 2.0f, 10.0f);
	unsigned int viewPosLocation = glGetUniformLocation(shaderProgramID, "viewPos");
	glUniform3f(viewPosLocation, eye.x, eye.y, eye.z);

	glm::mat4 view = glm::lookAt(eye,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

	return projection * view;
}

GLvoid updateTransformMatrix() {
	unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "Transform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Transform_matrix));
}

glm::mat4 getThirdPersonViewMatrix() {
	glm::vec3 eye = glm::vec3(0.0f, 10.0f, 20.0f);
	glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

	return glm::lookAt(eye, center, up);
}