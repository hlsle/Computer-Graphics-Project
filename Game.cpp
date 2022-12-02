#include "objRead.h"

#define VAO_SIZE 20
#define VBO_SIZE 100
#define GENERATOR 25
#define Z -Floor_obj.scaleZ * 2

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLchar* vertexsource, * fragmentsource; 
GLuint vertexshader, fragmentshader;
GLuint s_program;
void InitBuffer();
void InitShader();
char* filetobuf(const char* file);
void changeOpenGL(int x, int y, float* ox, float* oy);
void Keyboard(unsigned char key, int x, int y);
void Timer(int value);
void generateOb(int value);
float x_init, y_init, ox, oy, mx, my;
GLuint vao[VAO_SIZE], vbo[VBO_SIZE];
glm::vec3 cameraPos = glm::vec3(0.0f, 4.0f, 8.0f);
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); 
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 projection = glm::mat4(1.0f);
unsigned int projectionLocation;
unsigned int viewLocation;
objRead item_obj;
objRead Rock_obj;
objRead Bomb_obj;
objRead Thorn_obj;
objRead chara_obj;
objRead wall_obj;
objRead Floor_obj;
int Item_ver_cnt = item_obj.loadObj_normalize_center("Cube.obj");
int Bomb_ver_cnt = Bomb_obj.loadObj_normalize_center("Cube.obj");
int Rock_ver_cnt = Rock_obj.loadObj_normalize_center("Rock.obj");
int Thorn_ver_cnt = Thorn_obj.loadObj_normalize_center("Thorn.obj");
int Ch_ver_cnt = chara_obj.loadObj_normalize_center("Char.obj");
int Wall_ver_cnt = wall_obj.loadObj_normalize_center("Wall.obj");
int Floor_ver_cnt = Floor_obj.loadObj_normalize_center("Floor.obj");

vector<glm::vec3> item_col(36, glm::vec3(0.0, 0.0, 1.0));
vector<glm::vec3> rock_col(Rock_ver_cnt, glm::vec3(0.5, 0.5, 0.5));
vector<glm::vec3> Bomb_col(36, glm::vec3(0.0, 0.0, 0.0));
vector<glm::vec3> Thorn_col(Thorn_ver_cnt, glm::vec3(1.0, 0.0, 0.0));
vector<glm::vec3> chara_col(36, glm::vec3(0.0, 1.0, 0.0));
vector<glm::vec3> wall_col(36, glm::vec3(0.92, 0.92, 0.92));
vector<glm::vec3> Floor_col(Floor_ver_cnt, glm::vec3(0.96, 0.96, 0.96));

Map* map = new Map[3];
Map* map2 = new Map[3];
Map* map3 = new Map[3];
Character* ch = new Character();
vector<float> generator;
Rock* rock = new Rock[10];
Bomb* bomb = new Bomb[10];
Thorn* thorn = new Thorn[10];

glm::mat4 Trans;
int d = 1;
enum Obs {
	ROCK,
	BOMB,
	THORN
};
void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	srand((unsigned int)time(NULL));

	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 10);
	glutInitWindowSize(1100, 800);
	glutCreateWindow("Mini Ski");
	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();
	InitShader();
	InitBuffer();

	map[0].init_Pos(glm::vec3(0.0, 0.0, 0.0), 0.0);
	map[1].init_Pos(glm::vec3(-(Floor_obj.scaleX + wall_obj.scaleX) / 2, 0.0, 0.0), 0.0);
	map[2].init_Pos(glm::vec3((Floor_obj.scaleX + wall_obj.scaleX) / 2, 0.0, 0.0), 0.0);

	map2[0].init_Pos(glm::vec3(0.0, 0.0, -Floor_obj.scaleZ), 0.0);
	map2[1].init_Pos(glm::vec3(-(Floor_obj.scaleX + wall_obj.scaleX) / 2, 0.0, -Floor_obj.scaleZ), 0.0);
	map2[2].init_Pos(glm::vec3((Floor_obj.scaleX + wall_obj.scaleX) / 2, 0.0, -Floor_obj.scaleZ), 0.0);

	map3[0].init_Pos(glm::vec3(0.0, 0.0, Z), 0.0);
	map3[1].init_Pos(glm::vec3(-(Floor_obj.scaleX + wall_obj.scaleX) / 2, 0.0, Z), 0.0);
	map3[2].init_Pos(glm::vec3((Floor_obj.scaleX + wall_obj.scaleX) / 2, 0.0, Z), 0.0);
	
	for (int i = 0; i < GENERATOR; i++) {
		generator.push_back(-6.0 + 0.5 * i);
	}

	//원근 투영
	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.0f);

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(50, Timer, 1);
	glutTimerFunc(5000, generateOb, 1);
	glutMainLoop();
}
void make_vertexShader()
{
	vertexsource = filetobuf("vertex.glsl");
	//--- 버텍스 세이더 객체 만들기
	vertexshader = glCreateShader(GL_VERTEX_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(vertexshader, 1, (const GLchar**)&vertexsource, 0);
	//--- 버텍스 세이더 컴파일하기
	glCompileShader(vertexshader);
	//--- 컴파일이 제대로 되지 않은 경우: 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexshader, 512, NULL, errorLog);
		cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << endl;
		return;
	}
}
void make_fragmentShader()
{
	fragmentsource = filetobuf("fragment.glsl");
	//--- 프래그먼트 세이더 객체 만들기
	fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(fragmentshader, 1, (const GLchar**)&fragmentsource, 0);
	//--- 프래그먼트 세이더 컴파일
	glCompileShader(fragmentshader);
	//--- 컴파일이 제대로 되지 않은 경우: 컴파일 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentshader, 512, NULL, errorLog);
		cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << endl;
		return;
	}
}
void InitBuffer()
{
	glGenVertexArrays(VAO_SIZE, vao);
	glGenBuffers(VBO_SIZE, vbo);

	//item
	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, item_obj.outvertex.size() * sizeof(glm::vec3), &item_obj.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, item_col.size() * sizeof(glm::vec3), &item_col[0][0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	//rock
	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, Rock_obj.outvertex.size() * sizeof(glm::vec3), &Rock_obj.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, rock_col.size() * sizeof(glm::vec3), &rock_col[0][0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	//char
	glBindVertexArray(vao[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glBufferData(GL_ARRAY_BUFFER, chara_obj.outvertex.size() * sizeof(glm::vec3), &chara_obj.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
	glBufferData(GL_ARRAY_BUFFER, chara_col.size() * sizeof(glm::vec3), &chara_col[0][0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	//wall
	glBindVertexArray(vao[3]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);
	glBufferData(GL_ARRAY_BUFFER, wall_obj.outvertex.size() * sizeof(glm::vec3), &wall_obj.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[7]);
	glBufferData(GL_ARRAY_BUFFER, wall_col.size() * sizeof(glm::vec3), &wall_col[0][0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	//floor
	glBindVertexArray(vao[4]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[8]);
	glBufferData(GL_ARRAY_BUFFER, Floor_obj.outvertex.size() * sizeof(glm::vec3), &Floor_obj.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[9]);
	glBufferData(GL_ARRAY_BUFFER, Floor_col.size() * sizeof(glm::vec3), &Floor_col[0][0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	//bomb
	glBindVertexArray(vao[5]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[10]);
	glBufferData(GL_ARRAY_BUFFER, Bomb_obj.outvertex.size() * sizeof(glm::vec3), &Bomb_obj.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[11]);
	glBufferData(GL_ARRAY_BUFFER, Bomb_col.size() * sizeof(glm::vec3), &Bomb_col[0][0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	//thorn
	glBindVertexArray(vao[6]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[12]);
	glBufferData(GL_ARRAY_BUFFER, Thorn_obj.outvertex.size() * sizeof(glm::vec3), &Thorn_obj.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[13]);
	glBufferData(GL_ARRAY_BUFFER, Thorn_col.size() * sizeof(glm::vec3), &Thorn_col[0][0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
}
void InitShader()
{
	make_vertexShader(); //--- 버텍스 세이더 만들기
	make_fragmentShader(); //--- 프래그먼트 세이더 만들기
	//-- shader Program
	s_program = glCreateProgram();
	glAttachShader(s_program, vertexshader);
	glAttachShader(s_program, fragmentshader);
	glLinkProgram(s_program);
	//--- 세이더 삭제하기
	glDeleteShader(vertexshader);
	glDeleteShader(fragmentshader);
	//--- Shader Program 사용하기
	glUseProgram(s_program);
}
GLvoid drawScene()
{
	glViewport(0, 0, 1100, 800);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(s_program);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);

	viewLocation = glGetUniformLocation(s_program, "viewTransform");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);

	projectionLocation = glGetUniformLocation(s_program, "projectionTransform");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, value_ptr(projection));

	unsigned int modelLocation = glGetUniformLocation(s_program, "trans");
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	/*
	0 item
	1 rock
	2 character
	3 wall
	4 floor
	5 bomb
	6 thorn
	*/

	// item
	glBindVertexArray(vao[0]);

	// rock
	glBindVertexArray(vao[1]);
	for (int i = 0; i < 10; i++) {
		if (rock[i].created) {
			Trans = glm::translate(glm::mat4(1.0), rock[i].T);
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(Trans));
			glDrawArrays(GL_TRIANGLES, 0, Rock_ver_cnt);
		}
	}

	// bomb
	glBindVertexArray(vao[5]);
	for (int i = 0; i < 10; i++) {
		if (bomb[i].created) {
			Trans = glm::translate(glm::mat4(1.0), bomb[i].T);
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(Trans));
			glDrawArrays(GL_TRIANGLES, 0, Bomb_ver_cnt);
		}
	}

	// thorn
	glBindVertexArray(vao[6]);
	for (int i = 0; i < 10; i++) {
		if (thorn[i].created) {
			Trans = glm::translate(glm::mat4(1.0), thorn[i].T);
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(Trans));
			glDrawArrays(GL_TRIANGLES, 0, Thorn_ver_cnt);
		}
	}
	// character
	glBindVertexArray(vao[2]);
	Trans = glm::translate(glm::mat4(1.0), ch->T);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(Trans));
	glDrawArrays(GL_TRIANGLES, 0, Ch_ver_cnt);

	// map
	glBindVertexArray(vao[3]);
	for (int i = 1; i < 3; i++) {
		Trans = glm::translate(glm::mat4(1.0), map[i].T);
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(Trans));
		glDrawArrays(GL_TRIANGLES, 0, Wall_ver_cnt);
		Trans = glm::translate(glm::mat4(1.0), map2[i].T);
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(Trans));
		glDrawArrays(GL_TRIANGLES, 0, Wall_ver_cnt);
		Trans = glm::translate(glm::mat4(1.0), map3[i].T);
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(Trans));
		glDrawArrays(GL_TRIANGLES, 0, Wall_ver_cnt);
	}

	glBindVertexArray(vao[4]);
	Trans = glm::translate(glm::mat4(1.0), map[0].T);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(Trans));
	glDrawArrays(GL_TRIANGLES, 0, Floor_ver_cnt);
	Trans = glm::translate(glm::mat4(1.0), map2[0].T);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(Trans));
	glDrawArrays(GL_TRIANGLES, 0, Floor_ver_cnt);
	Trans = glm::translate(glm::mat4(1.0), map3[0].T);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(Trans));
	glDrawArrays(GL_TRIANGLES, 0, Floor_ver_cnt);

	glutSwapBuffers();
}
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
{
	glViewport(0, 0, w, h);
}
void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'd':
		if (d == 1)
			d = 2;
		else d = 1;
		for (int i = 0; i < 3; i++) {
			map[i].dir = d;
			map2[i].dir = d;
			map3[i].dir = d;
		}
		for (int i = 0; i < 10; i++) {
			rock[i].dir = d;
			bomb[i].dir = d;
			thorn[i].dir = d;
		}
		break;
	}
	glutPostRedisplay();
}
void Timer(int value) {
	for (int i = 0; i < 3; i++) {
		map[i].Move();
		map2[i].Move();
		map3[i].Move();
	}
	
	if (map[0].T.z >= 17) {
		for (int i = 0; i < 3; i++) {
			map[i].T.z = Z;
		}
	}
	if (map2[0].T.z >= 17) {
		for (int i = 0; i < 3; i++) {
			map2[i].T.z = Z;
		}
	}
	if (map3[0].T.z >= 17) {
		for (int i = 0; i < 3; i++) {
			map3[i].T.z = Z;
		}
	}
	for (int i = 0; i < 10; i++) {
		if (rock[i].created)
			rock[i].Move();

		if (rock[i].T.z > 18)
			rock[i].T.z = Z;

		if (bomb[i].created)
			bomb[i].Move();

		if (bomb[i].T.z > 18)
			bomb[i].T.z = Z;

		if (thorn[i].created)
			thorn[i].Move();

		if (thorn[i].T.z > 18)
			thorn[i].T.z = Z;
	}

	if(d ==1)
		for (int i = 0; i < GENERATOR; i++) {
			generator[i] -= 0.1;
		}
	else
		for (int i = 0; i < GENERATOR; i++) {
			generator[i] += 0.1;
		}

	glutTimerFunc(100, Timer, 1);
	glutPostRedisplay();
}
void generateOb(int value) {

	int kinds = rand() % 3;
	int idx = rand() % 25;
	switch (kinds) {
	case Obs::ROCK:
		for (int i = 0; i < 10; i++) {
			if (!rock[i].created) {
				rock[i].init_pos(glm::vec3(generator[idx], 0.0, -Floor_obj.scaleZ * 2), 0.0);
				rock[i].created = true;
				break;
			}
		}
		break;
	case Obs::BOMB:
		for (int i = 0; i < 10; i++) {
			if (!bomb[i].created) {
				bomb[i].init_pos(glm::vec3(generator[idx], 0.0, -Floor_obj.scaleZ * 2), 0.0);
				bomb[i].created = true;
				break;
			}
		}
		break;
	case Obs::THORN:
		for (int i = 0; i < 10; i++) {
			if (!thorn[i].created) {
				thorn[i].init_pos(glm::vec3(generator[idx], 0.0, -Floor_obj.scaleZ * 2), 0.0);
				thorn[i].created = true;
				break;
			}
		}
		break;
	}

	// 5초당 생성
	glutTimerFunc(5000, generateOb, 1);
	glutPostRedisplay();
}
//opengl 좌표로 변환
void changeOpenGL(int x, int y, float* ox, float* oy)
{
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	*ox = (float)((x - (float)w / 2.0) * (float)(1.0 / (float)(w / 2.0)));
	*oy = -(float)((y - (float)h / 2.0) * (float)(1.0 / (float)(h / 2.0)));
}
char* filetobuf(const char* file)
{
	FILE* fptr;
	long length;
	char* buf;
	fptr = fopen(file, "rb"); // Open file for reading
	if (!fptr) // Return NULL on failure
		return NULL;
	fseek(fptr, 0, SEEK_END); // Seek to the end of the file
	length = ftell(fptr); // Find out how many bytes into the file we are
	buf = (char*)malloc(length + 1); // Allocate a buffer for the entire length of the file and a null terminator
	fseek(fptr, 0, SEEK_SET); // Go back to the beginning of the file
	fread(buf, length, 1, fptr); // Read the contents of the file in to the buffer
	fclose(fptr); // Close the file
	buf[length] = 0; // Null terminator
	return buf; // Return the buffer
}