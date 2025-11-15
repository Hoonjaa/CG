#include "pch.h"
#include "Coordinate_system.h"
#include "Stage.h"
#include "TreeNode.h"
#include "Robot_part.h"
#include "Robot_body.h"
#include "Robot_arm.h"
#include "Robot_head.h"
#include "Robot_nose.h"
#include "Obstacle.h"

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
bool Timer = true;
glm::mat4 Transform_matrix{ 1.0f };

Coordinate_system* coordinate_system = nullptr;

//무대 관련 변수
Stage* stage = nullptr;
bool stage_open = false;

//장애물 관련 변수
Obstacle* obstacle1 = nullptr;
Obstacle* obstacle2 = nullptr;
Obstacle* obstacle3 = nullptr;

//카메라 관련 변수
GLfloat camera_x = 0.0f, camera_z = 20.0f;
GLfloat camera_y_rotation = 0.0f;		//카메라 자전
GLfloat camera_orbit_rotation = 0.0f;	//카메라 공전

//로봇 클래스
class Robot {
private:
	std::shared_ptr<TreeNode> root;
	std::shared_ptr<Robot_Part> bodyPart;
	std::shared_ptr<Robot_Part> RightArmPart;
	std::shared_ptr<Robot_Part> LeftArmPart;
	std::shared_ptr<Robot_Part> RightLegPart;
	std::shared_ptr<Robot_Part> LeftLegPart;
	std::shared_ptr<Robot_Part> headPart;
	std::shared_ptr<Robot_Part> nosePart;

	glm::vec3 position{ 0.0f, 0.0f, 0.0f };
	GLfloat walk_angle = 0.0f;
	GLfloat rotation_angle = 0.0f;
	//걷는 모션
	GLfloat limb_rotation = 0.0f;
	bool limb_forward = true;
	GLfloat walk_speed = 0.03f;
	GLfloat MAX_LIMB_ROTATION = glm::radians(20.0f);

	// 점프 관련 변수
	bool isJumping = false;
	bool isFalling = false;
	GLfloat jump_velocity = 0.14f;
	GLfloat gravity = 0.005f;
	GLfloat max_jump_height = 2.0f;
	GLfloat initial_y = 0.0f;

public:
	GLvoid setup(GLuint shader) {
		root = std::make_shared<TreeNode>();

		//몸체 생성
		auto body = std::make_shared<Robot_body>();
		bodyPart = std::make_shared<Robot_Part>(body, shader);
		bodyPart->translate(glm::vec3(0.0f, -3.3f, 0.0f));
		root->addChild(bodyPart);

		//오른팔 생성
		auto Rarm = std::make_shared<Robot_arm>();
		RightArmPart = std::make_shared<Robot_Part>(Rarm, shader);
		RightArmPart->translate(glm::vec3(-0.5f, 0.1f, 0.0f));
		bodyPart->addChild(RightArmPart);

		//왼팔 생성
		auto Larm = std::make_shared<Robot_arm>();
		Larm->set_color(glm::vec3(0.0f, 0.6f, 0.6f)); //왼팔 파란색으로 설정
		LeftArmPart = std::make_shared<Robot_Part>(Larm, shader);
		LeftArmPart->translate(glm::vec3(0.5f, 0.1f, 0.0f));
		bodyPart->addChild(LeftArmPart);

		//오른다리 생성
		auto Rleg = std::make_shared<Robot_arm>();
		Rleg->set_color(glm::vec3(0.0f, 0.6f, 0.6f)); //오른다리 보라색으로 설정
		RightLegPart = std::make_shared<Robot_Part>(Rleg, shader);
		RightLegPart->translate(glm::vec3(-0.2f, -1.1f, 0.0f));
		bodyPart->addChild(RightLegPart);

		//왼다리 생성
		auto Lleg = std::make_shared<Robot_arm>();
		LeftLegPart = std::make_shared<Robot_Part>(Lleg, shader);
		LeftLegPart->translate(glm::vec3(0.2f, -1.1f, 0.0f));
		bodyPart->addChild(LeftLegPart);

		//머리 생성
		auto head = std::make_shared<Robot_head>();
		headPart = std::make_shared<Robot_Part>(head, shader);
		headPart->translate(glm::vec3(0.0f, 0.9f, 0.0f));
		bodyPart->addChild(headPart);

		//코 생성
		auto nose = std::make_shared<Robot_nose>();
		nosePart = std::make_shared<Robot_Part>(nose, shader);
		nosePart->translate(glm::vec3(0.0f, -0.05f, 0.25f));
		headPart->addChild(nosePart);
	}

	GLvoid render(const glm::mat4& viewProjectionMatrix) {
		if (root) {
			glm::mat4 robotTransform = glm::translate(glm::mat4(1.0f), position);
			robotTransform = glm::rotate(robotTransform, rotation_angle, glm::vec3(0.0f, 1.0f, 0.0f));

			glm::mat4 finalTransform = viewProjectionMatrix * robotTransform;
			root->render(finalTransform);
		}
	}

	GLvoid Walk() {
		// 다음 이동할 위치 미리 계산
		glm::vec3 nextPosition = position;
		nextPosition.x += walk_speed * sin(rotation_angle);
		nextPosition.z += walk_speed * cos(rotation_angle);

		// 다음 위치에서의 바운딩 박스 계산
		glm::vec3 nextMin = nextPosition + glm::vec3(-0.5f, -5.0f, -0.5f);
		glm::vec3 nextMax = nextPosition + glm::vec3(0.5f, -3.0f, 0.5f);

		// 다음 위치에서 충돌 체크
		bool willCollide = false;

		if (checkCollisionWithBox(nextMin, nextMax, obstacle1->getMin(), obstacle1->getMax()) ||
			checkCollisionWithBox(nextMin, nextMax, obstacle2->getMin(), obstacle2->getMax()) ||
			checkCollisionWithBox(nextMin, nextMax, obstacle3->getMin(), obstacle3->getMax())) {
			willCollide = true;
		}

		// 충돌하지 않을 때만 이동
		if (!willCollide) {
			position = nextPosition;
		}

		// 현재 위치에서 장애물 체크 (발 아래)
		bool onObstacle = false;
		glm::vec3 currentMin = position + glm::vec3(-0.5f, -5.0f, -0.5f);
		glm::vec3 currentMax = position + glm::vec3(0.5f, -3.0f, 0.5f);

		if (checkCollisionWithBox(currentMin, currentMax, obstacle1->getMin(), obstacle1->getMax()) ||
			checkCollisionWithBox(currentMin, currentMax, obstacle2->getMin(), obstacle2->getMax()) ||
			checkCollisionWithBox(currentMin, currentMax, obstacle3->getMin(), obstacle3->getMax())) {
			onObstacle = true;
		}

		// 장애물 위에 있지 않고, 점프/낙하 중이 아니며, 바닥보다 높은 곳에 있으면 낙하 시작
		if (!onObstacle && !isJumping && !isFalling && position.y > 0.0f) {
			isFalling = true;
			initial_y = 0.0f;  // 목표 바닥 높이를 0으로 설정
			jump_velocity = 0.0f;
		}
		

		// 팔다리 회전 각도 업데이트
		if (limb_forward) {
			limb_rotation += glm::radians(2.0f);
			if (limb_rotation >= MAX_LIMB_ROTATION) {
				limb_rotation = MAX_LIMB_ROTATION;
				limb_forward = false;
			}
		}
		else {
			limb_rotation -= glm::radians(2.0f);
			if (limb_rotation <= -MAX_LIMB_ROTATION) {
				limb_rotation = -MAX_LIMB_ROTATION;
				limb_forward = true;
			}
		}

		// 오른팔 - 상단(어깨) 기준 회전
		if (RightArmPart) {
			glm::vec3 rightArmPos(-0.5f, 0.1f, 0.0f);
			glm::vec3 pivotOffset(0.0f, 0.5f, 0.0f); // 팔 길이의 절반만큼 위로 (상단)

			RightArmPart->setTransform(glm::mat4(1.0f));
			RightArmPart->translate(rightArmPos);
			RightArmPart->translate(pivotOffset); // 회전축으로 이동
			RightArmPart->rotate(limb_rotation, glm::vec3(1.0f, 0.0f, 0.0f));
			RightArmPart->translate(-pivotOffset); // 원위치
		}

		// 왼팔 - 상단(어깨) 기준 회전
		if (LeftArmPart) {
			glm::vec3 leftArmPos(0.5f, 0.1f, 0.0f);
			glm::vec3 pivotOffset(0.0f, 0.5f, 0.0f);

			LeftArmPart->setTransform(glm::mat4(1.0f));
			LeftArmPart->translate(leftArmPos);
			LeftArmPart->translate(pivotOffset);
			LeftArmPart->rotate(-limb_rotation, glm::vec3(1.0f, 0.0f, 0.0f));
			LeftArmPart->translate(-pivotOffset);
		}

		// 오른다리 - 상단(골반) 기준 회전
		if (RightLegPart) {
			glm::vec3 rightLegPos(-0.2f, -1.1f, 0.0f);
			glm::vec3 pivotOffset(0.0f, 0.5f, 0.0f); // 다리 길이의 절반만큼 위로 (상단)

			RightLegPart->setTransform(glm::mat4(1.0f));
			RightLegPart->translate(rightLegPos);
			RightLegPart->translate(pivotOffset);
			RightLegPart->rotate(-limb_rotation, glm::vec3(1.0f, 0.0f, 0.0f));
			RightLegPart->translate(-pivotOffset);
		}

		// 왼다리 - 상단(골반) 기준 회전
		if (LeftLegPart) {
			glm::vec3 leftLegPos(0.2f, -1.1f, 0.0f);
			glm::vec3 pivotOffset(0.0f, 0.5f, 0.0f);

			LeftLegPart->setTransform(glm::mat4(1.0f));
			LeftLegPart->translate(leftLegPos);
			LeftLegPart->translate(pivotOffset);
			LeftLegPart->rotate(limb_rotation, glm::vec3(1.0f, 0.0f, 0.0f));
			LeftLegPart->translate(-pivotOffset);
		}
	}

	GLvoid RotateLeft() {
		// 왼쪽으로 90도 회전
		rotation_angle += glm::radians(90.0f);
	}

	GLvoid RotateRight() {
		// 오른쪽으로 90도 회전
		rotation_angle -= glm::radians(90.0f);
	}

	GLvoid resetMotion() {
		GLfloat limb_rotation = 0.0f;
		bool limb_forward = true;

		if (RightArmPart) {
			glm::vec3 rightArmPos(-0.5f, 0.1f, 0.0f);
			RightArmPart->setTransform(glm::mat4(1.0f));
			RightArmPart->translate(rightArmPos);
		}

		// 왼팔 초기 위치로 복원
		if (LeftArmPart) {
			glm::vec3 leftArmPos(0.5f, 0.1f, 0.0f);
			LeftArmPart->setTransform(glm::mat4(1.0f));
			LeftArmPart->translate(leftArmPos);
		}

		// 오른다리 초기 위치로 복원
		if (RightLegPart) {
			glm::vec3 rightLegPos(-0.2f, -1.1f, 0.0f);
			RightLegPart->setTransform(glm::mat4(1.0f));
			RightLegPart->translate(rightLegPos);
		}

		// 왼다리 초기 위치로 복원
		if (LeftLegPart) {
			glm::vec3 leftLegPos(0.2f, -1.1f, 0.0f);
			LeftLegPart->setTransform(glm::mat4(1.0f));
			LeftLegPart->translate(leftLegPos);
		}
	}

	GLvoid increaseSpeed() {
		walk_speed += 0.01f;
		MAX_LIMB_ROTATION += glm::radians(5.0f);
	}

	GLvoid decreaseSpeed() {
		walk_speed -= 0.01f;
		if (walk_speed < 0.01f) walk_speed = 0.01f;  // 최소 속도 제한

		MAX_LIMB_ROTATION -= glm::radians(5.0f);
		if (MAX_LIMB_ROTATION < glm::radians(5.0f)) {
			MAX_LIMB_ROTATION = glm::radians(5.0f);  // 최소 회전 각도 제한
		}
	}

	GLvoid reset() {
		// 위치 초기화
		position = glm::vec3(0.0f, 0.0f, 0.0f);

		// 회전 각도 초기화
		rotation_angle = 0.0f;
		walk_angle = 0.0f;

		// 걷기 모션 초기화
		resetMotion();

		// 속도 및 회전 각도 초기화
		walk_speed = 0.03f;
		MAX_LIMB_ROTATION = glm::radians(20.0f);

		// 점프 상태 초기화
		isJumping = false;
		isFalling = false;
		jump_velocity = 0.14f;
	}

	GLvoid Jump() {
		if (!isJumping && !isFalling) {
			isJumping = true;
			initial_y = position.y;
			jump_velocity = 0.14f;
		}

		if (isJumping) {
			// 위로 이동
			position.y += jump_velocity;
			jump_velocity -= gravity;

			// 최대 높이 도달 또는 속도가 0 이하가 되면 낙하 시작
			if (jump_velocity <= 0.0f || position.y >= initial_y + max_jump_height) {
				if (position.y > initial_y + max_jump_height) {
					position.y = initial_y + max_jump_height;
				}
				isJumping = false;
				isFalling = true;
				jump_velocity = 0.0f;  // 낙하 시작 시 속도 0으로 초기화
			}
		}
	}

	GLvoid Fall() {
		if (isFalling) {
			// 아래로 이동
			jump_velocity += gravity;
			position.y -= jump_velocity;

			// 장애물과의 충돌 체크
			bool collided = false;
			if (checkCollision(obstacle1->getMin(), obstacle1->getMax()) ||
				checkCollision(obstacle2->getMin(), obstacle2->getMax()) ||
				checkCollision(obstacle3->getMin(), obstacle3->getMax())) {
				collided = true;
			}

			// 충돌 시 바닥 레벨을 장애물 위로 설정
			if (collided) {
				position.y = initial_y + 0.81f;
				initial_y = position.y;  // 새로운 바닥 레벨 저장
				isFalling = false;
				jump_velocity = 0.0f;
			}
			// 원래 바닥(0.0f)에 도달 확인
			else if (position.y <= 0.0f) {
				position.y = 0.0f;
				initial_y = 0.0f;  // 바닥 레벨을 원래대로 복원
				isFalling = false;
				jump_velocity = 0.0f;
			}
		}
	}

	GLvoid updateJump() {
		if (isJumping) {
			Jump();
		}
		else if (isFalling) {
			Fall();
		}
	}

	bool getIsJumping() const { return isJumping || isFalling; }

	glm::vec3 getBoundingBoxMin() const {
		return position + glm::vec3(-0.5f, -5.0f, -0.5f);
	}
	glm::vec3 getBoundingBoxMax() const {
		return position + glm::vec3(0.5f, -3.0f, 0.5f);
	}

	bool checkCollision(const glm::vec3& obsMin, const glm::vec3& obsMax) {
		glm::vec3 robotMin = getBoundingBoxMin();
		glm::vec3 robotMax = getBoundingBoxMax();
		bool collisionX = robotMax.x >= obsMin.x && robotMin.x <= obsMax.x;
		bool collisionY = robotMax.y >= obsMin.y && robotMin.y <= obsMax.y;
		bool collisionZ = robotMax.z >= obsMin.z && robotMin.z <= obsMax.z;
		
		return collisionX && collisionY && collisionZ;
	}

	bool checkCollisionWithBox(const glm::vec3& boxMin1, const glm::vec3& boxMax1,
		const glm::vec3& boxMin2, const glm::vec3& boxMax2) {
		bool collisionX = boxMax1.x >= boxMin2.x && boxMin1.x <= boxMax2.x;
		bool collisionY = boxMax1.y >= boxMin2.y && boxMin1.y <= boxMax2.y;
		bool collisionZ = boxMax1.z >= boxMin2.z && boxMin1.z <= boxMax2.z;

		return collisionX && collisionY && collisionZ;
	}
};

//로봇 관련 변수
Robot* robot = nullptr;
bool walking = false;


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
	glFrontFace(GL_CCW);
	coordinate_system = new Coordinate_system();
	stage = new Stage();
	robot = new Robot();
	robot->setup(shaderProgramID);
	
	obstacle1 = new Obstacle();
	obstacle2 = new Obstacle();
	obstacle3 = new Obstacle();

	glutTimerFunc(16, TimerFunction, 1);
	setViewPerspectiveMatrix();
	glutDisplayFunc(drawScene);											//--- 출력 콜백 함수
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutKeyboardUpFunc(KeyboardUp);
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

	glFrontFace(GL_CW);
	if (stage) stage->draw(shaderProgramID, Transform_matrix);

	glFrontFace(GL_CCW);
	if (robot) robot->render(Transform_matrix);

	if (obstacle1) obstacle1->draw(shaderProgramID, Transform_matrix);
	if (obstacle2) obstacle2->draw(shaderProgramID, Transform_matrix);
	if (obstacle3) obstacle3->draw(shaderProgramID, Transform_matrix);

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
	case'o':
		stage_open = true;
		break;
	case'x':
		camera_x += 0.5f;
		setViewPerspectiveMatrix();
		break;
	case'X':
		camera_x -= 0.5f;
		setViewPerspectiveMatrix();
		break;
	case'z':
		camera_z += 0.5f;
		setViewPerspectiveMatrix();
		break;
	case'Z':
		camera_z -= 0.5f;
		setViewPerspectiveMatrix();
		break;
	case'y':
		camera_orbit_rotation += glm::radians(5.0f);
		setViewPerspectiveMatrix();
		break;
	case'Y':
		camera_orbit_rotation -= glm::radians(5.0f);
		setViewPerspectiveMatrix();
		break;
	case 'w':
		walking = true;
		break;
	case 'a':
		if (robot) robot->RotateLeft();
		break;
	case 'd':
		if (robot) robot->RotateRight();
		break;
	case 's':
		if (robot) robot->RotateLeft();
		if (robot) robot->RotateLeft();
		break;
	case '+':
	case '=':
		if (robot) robot->increaseSpeed();
		break;
	case '-':
	case '_':
		if (robot) robot->decreaseSpeed();
		break;
	case 'i':
		if (robot) robot->reset();
		break;
	case 'j':
		if (robot && !robot->getIsJumping()) {
			robot->Jump();
		}
		break;
	}
	glutPostRedisplay();
}

GLvoid KeyboardUp(unsigned char key, int x, int y) {
	switch (key) {
	case 'w':
		walking = false;
		robot->resetMotion();
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
	if (robot) {
		robot->updateJump();
	}
	if (walking) {
		if (robot) robot->Walk();
	}
	if (stage_open) {
		stage->front_angle += 1.0f;
		if (stage->front_angle >= 120.0f) {
			stage_open = false;
			glEnable(GL_CULL_FACE);
			stage->front_angle = 0.0f;
		}
	}
	glutPostRedisplay();
	if (Timer) {
		glutTimerFunc(16, TimerFunction, 1);
	}
}

GLvoid setViewPerspectiveMatrix() {
	// 1. 먼저 공전(orbit) 적용: 카메라의 기본 위치를 원점 중심으로 회전
	GLfloat orbited_x = camera_x * cos(camera_orbit_rotation) - camera_z * sin(camera_orbit_rotation);
	GLfloat orbited_z = camera_x * sin(camera_orbit_rotation) + camera_z * cos(camera_orbit_rotation);

	// 공전된 카메라 위치
	glm::vec3 cameraPos(orbited_x, 0.0f, orbited_z);

	// 2. 자전(rotation) 적용: 카메라가 바라보는 방향을 회전
	// 초기 시선 방향 (원점을 향함)
	glm::vec3 lookDirection = glm::normalize(glm::vec3(0.0f, 0.0f, 0.0f) - cameraPos);

	// y축을 중심으로 시선 방향 회전
	glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), camera_y_rotation, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec4 rotatedDirection = rotationMatrix * glm::vec4(lookDirection, 0.0f);

	// 카메라가 바라볼 목표점 = 카메라 위치 + 회전된 방향
	glm::vec3 target = cameraPos + glm::vec3(rotatedDirection);

	glm::mat4 view = glm::lookAt(cameraPos, target, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

	Transform_matrix = projection * view;
	updateTransformMatrix();
}

GLvoid updateTransformMatrix() {
	unsigned int modelLocation = glGetUniformLocation(shaderProgramID, "Transform");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(Transform_matrix));
}