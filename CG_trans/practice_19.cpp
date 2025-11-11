#include "pch.h"
#include "Coordinate_system.h"
#include "Planet.h"
#include "Orbit.h"

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
glm::mat4 getViewPerspectiveMatrix();
// 변환 행렬 업데이트
GLvoid updateTransformMatrix();

//--- 필요한 변수 선언
GLuint shaderProgramID;													//--- 세이더 프로그램 이름
GLuint vertexShader;													//--- 버텍스 세이더 객체
GLuint fragmentShader;													//--- 프래그먼트 세이더 객체

GLint WindowWidth = 800, WindowHeight = 800;
bool Timer = true;
glm::mat4 Transform_matrix{ 1.0f };

Coordinate_system* coordinate_system = nullptr;
//행성에 관한 변수들
Planet* central_planet = nullptr;

struct planetTransform {
	Planet* planet;
	GLfloat angle;
	glm::vec3 axis;
};
std::vector <planetTransform> planets;
Planet* middle_planet_1 = nullptr;
Planet* middle_planet_2 = nullptr;
Planet* middle_planet_3 = nullptr;

// 3단계 행성들
std::vector <planetTransform> third_planets;
Planet* small_planet_1 = nullptr;
Planet* small_planet_2 = nullptr;
Planet* small_planet_3 = nullptr;

//궤도에 관한 변수들
struct OrbitTransform {
	Orbit* orbit;
	GLfloat angle;
	glm::vec3 axis;
};
std::vector <OrbitTransform> orbits;
std::vector <OrbitTransform> second_orbits;
Orbit* central_orbit_1 = nullptr;
Orbit* central_orbit_2 = nullptr;
Orbit* central_orbit_3 = nullptr;
Orbit* second_orbit_1 = nullptr;
Orbit* second_orbit_2 = nullptr;
Orbit* second_orbit_3 = nullptr;

GLfloat central_orbit_radius = 2.0f;
GLfloat second_orbit_radius = central_orbit_radius / 2.0f;

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
	glutTimerFunc(16, TimerFunction, 1);
	coordinate_system = new Coordinate_system();
	//--- 행성 만들기
	central_planet = new Planet(0.5f, 36, 18, {0.0f, 0.0f, 1.0f});
	middle_planet_1 = new Planet(0.25f, 36, 18, { 0.0f, 1.0f, 0.0f }, { central_orbit_radius, 0.0f, 0.0f });
	middle_planet_2 = new Planet(0.25f, 36, 18, { 0.0f, 1.0f, 0.0f }, { central_orbit_radius, 0.0f, 0.0f });
	middle_planet_3 = new Planet(0.25f, 36, 18, { 0.0f, 1.0f, 0.0f }, { central_orbit_radius, 0.0f, 0.0f });
	planets.push_back({ middle_planet_1, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f) });
	planets.push_back({ middle_planet_2, 45.0f, glm::vec3(0.0f, 0.0f, 1.0f) });
	planets.push_back({ middle_planet_3, -45.0f, glm::vec3(0.0f, 0.0f, 1.0f) });
	
	// 3단계 행성 만들기 (반지름 0.1, 원점에서 생성)
	small_planet_1 = new Planet(0.1f, 36, 18, { 1.0f, 0.0f, 0.0f }, { second_orbit_radius, 0.0f, 0.0f });
	small_planet_2 = new Planet(0.1f, 36, 18, { 1.0f, 0.0f, 0.0f }, { second_orbit_radius, 0.0f, 0.0f });
	small_planet_3 = new Planet(0.1f, 36, 18, { 1.0f, 0.0f, 0.0f }, { second_orbit_radius, 0.0f, 0.0f });
	third_planets.push_back({ small_planet_1, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f) });
	third_planets.push_back({ small_planet_2, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f) });
	third_planets.push_back({ small_planet_3, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f) });
	
	//--- 궤도 만들기
	central_orbit_1 = new Orbit(central_orbit_radius);
	central_orbit_2 = new Orbit(central_orbit_radius);
	central_orbit_3 = new Orbit(central_orbit_radius);
	orbits.push_back({ central_orbit_1, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f) });
	orbits.push_back({ central_orbit_2, 45.0f, glm::vec3(0.0f, 0.0f, 1.0f) });
	orbits.push_back({ central_orbit_3, -45.0f, glm::vec3(0.0f, 0.0f, 1.0f) });
	second_orbit_1 = new Orbit(second_orbit_radius);
	second_orbit_2 = new Orbit(second_orbit_radius);
	second_orbit_3 = new Orbit(second_orbit_radius);
	second_orbits.push_back({ second_orbit_1, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f) });
	second_orbits.push_back({ second_orbit_2, 45.0f, glm::vec3(0.0f, 0.0f, 1.0f) });
	second_orbits.push_back({ second_orbit_3, -45.0f, glm::vec3(0.0f, 0.0f, 1.0f) });


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

//--- 출력 콸백 함수
GLvoid drawScene()														//--- 콜백 함수: 그리기 콜백 함수
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramID);
	glPointSize(5.0);

	//if (coordinate_system) coordinate_system->draw(shaderProgramID, Transform_matrix);
	
	// 중심 행성 그리기
	if (central_planet) central_planet->draw(shaderProgramID, Transform_matrix);

	// 1단계 궤도 그리기
	for (const auto& orbitData : orbits) {
		if (!orbitData.orbit) continue;

		glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(orbitData.angle), orbitData.axis);
		Transform_matrix = getViewPerspectiveMatrix() * model;
		updateTransformMatrix();
		orbitData.orbit->draw(shaderProgramID, Transform_matrix);
	}

	// 각 1단계 궤도 시스템 (2단계 행성 + 2단계 궤도 + 3단계 행성)
	for (size_t i = 0; i < planets.size(); ++i) {
		const auto& planetData = planets[i];
		if (!planetData.planet) continue;

		// 1단계 궤도 회전 (기울기)
		glm::mat4 orbit_rotation = glm::rotate(glm::mat4(1.0f), glm::radians(planetData.angle), planetData.axis);
		
		// 2단계 행성의 공전
		glm::mat4 planet_revolution = glm::rotate(glm::mat4(1.0f), glm::radians(planetData.planet->revolve_angle), glm::vec3(0.0f, 1.0f, 0.0f));
		
		// 2단계 행성의 최종 변환 = 궤도 기울기 * 공전
		glm::mat4 planet_model = orbit_rotation * planet_revolution;
		Transform_matrix = getViewPerspectiveMatrix() * planet_model;
		updateTransformMatrix();
		planetData.planet->draw(shaderProgramID, Transform_matrix);
		
		// 2단계 궤도 그리기 (2단계 행성 위치에서)
		if (i < second_orbits.size() && second_orbits[i].orbit) {
			glm::mat4 third_revolution = glm::mat4(1.0f);
			third_revolution = glm::translate(third_revolution, planetData.planet->getPosition()); // 2단계 행성 위치로 이동
			Transform_matrix = getViewPerspectiveMatrix() * planet_model * third_revolution;
			updateTransformMatrix();
			second_orbits[i].orbit->draw(shaderProgramID, Transform_matrix);
		}
		
		// 3단계 행성 그리기 (2단계 행성 주위를 공전)
		if (i < third_planets.size() && third_planets[i].planet) {
			// 3단계 행성의 공전
			glm::mat4 third_revolution = glm::mat4(1.0f);
			third_revolution = glm::translate(third_revolution, planetData.planet->getPosition()); // 2단계 행성 위치로 이동
			third_revolution = glm::rotate(third_revolution, glm::radians(third_planets[i].planet->revolve_angle), glm::vec3(0.0f, 1.0f, 0.0f));
			
			// 3단계 행성의 최종 변환 = 궤도 기울기 * 2단계 공전 * 3단계 공전
			glm::mat4 third_model = planet_model * third_revolution;
			Transform_matrix = getViewPerspectiveMatrix() * third_model;
			updateTransformMatrix();
			third_planets[i].planet->draw(shaderProgramID, Transform_matrix);
		}
	}

	Transform_matrix = getViewPerspectiveMatrix();
	updateTransformMatrix();

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
	case 'm':
		central_planet->solid_mode = !central_planet->solid_mode;
		middle_planet_1->solid_mode = !middle_planet_1->solid_mode;
		middle_planet_2->solid_mode = !middle_planet_2->solid_mode;
		middle_planet_3->solid_mode = !middle_planet_3->solid_mode;
		small_planet_1->solid_mode = !small_planet_1->solid_mode;
		small_planet_2->solid_mode = !small_planet_2->solid_mode;
		small_planet_3->solid_mode = !small_planet_3->solid_mode;
		break;
	case 'y':
		central_orbit_1->radius += 0.1f; central_orbit_1->update();
		central_orbit_2->radius += 0.1f; central_orbit_2->update();
		central_orbit_3->radius += 0.1f; central_orbit_3->update();
		second_orbit_1->radius += 0.05f; second_orbit_1->update();
		second_orbit_2->radius += 0.05f; second_orbit_2->update();
		second_orbit_3->radius += 0.05f; second_orbit_3->update();
		break;
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
	for (auto& planetData : planets) {
		planetData.planet->revolve_angle += planetData.planet->revolve_speed;
		if (planetData.planet->revolve_angle >= 360.0f || planetData.planet->revolve_angle <= -360.0f)
			planetData.planet->revolve_angle = 0.0f;
	}
	
	// 3단계 행성 공전 업데이트
	for (auto& planetData : third_planets) {
		planetData.planet->revolve_angle += planetData.planet->revolve_speed;
		if (planetData.planet->revolve_angle >= 360.0f || planetData.planet->revolve_angle <= -360.0f)
			planetData.planet->revolve_angle = 0.0f;
	}
	
	glutPostRedisplay();
	if (Timer) {
		glutTimerFunc(16, TimerFunction, 1);
	}
}

GLvoid setViewPerspectiveMatrix() {
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 2.0f, 8.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

	Transform_matrix = projection * view;
	updateTransformMatrix();
}

glm::mat4 getViewPerspectiveMatrix() {
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 2.0f, 8.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

	return projection * view;
}

GLvoid updateTransformMatrix() {
	unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "Transform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Transform_matrix));
}