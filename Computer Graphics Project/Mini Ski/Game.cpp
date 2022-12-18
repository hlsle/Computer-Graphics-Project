#include "objRead.h"

#define VAO_SIZE 35
#define VBO_SIZE 100
#define GENERATOR 25
#define Z -Floor_obj.scaleZ * 3

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
void Jump(int value);
void generateOb(int value);
bool chara_collision_cal(glm::vec3 a, float scaleX, float scaleY, float scaleZ);
void Collision_check(int value);
bool wall_collid(int type, glm::vec3 v);
void Change_Direction();
void initkeyarray();
float x_init, y_init, ox, oy, mx, my;
GLuint vao[VAO_SIZE], vbo[VBO_SIZE];

glm::vec3 cameraPos = glm::vec3(0.0f, 4.0f, 8.0f);
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); 
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 projection = glm::mat4(1.0f);
unsigned int projectionLocation;
unsigned int viewLocation;

objRead Point_obj;
objRead Heal_obj;
objRead Shield_obj;
objRead Rock_obj;
objRead Bomb_obj;
objRead Thorn_obj;
objRead chara_obj;
objRead wall_obj;
objRead wall2_obj;
objRead Floor_obj;
objRead Jumpbar_obj;

int point_ver_cnt = Point_obj.loadObj_normalize_center("Coin.obj");
int heal_ver_cnt = Heal_obj.loadObj_normalize_center("Heart.obj");
int shield_ver_cnt = Shield_obj.loadObj_normalize_center("Shield.obj");
int Bomb_ver_cnt = Bomb_obj.loadObj_normalize_center("Bomb.obj");
int Rock_ver_cnt = Rock_obj.loadObj_normalize_center("Rock.obj");
int Thorn_ver_cnt = Thorn_obj.loadObj_normalize_center("Thorn.obj");
int Ch_ver_cnt = chara_obj.loadObj_normalize_center("Char.obj");
int Wall_ver_cnt = wall_obj.loadObj_normalize_center("Wall.obj");
int Wall2_ver_cnt = wall2_obj.loadObj_normalize_center("Wall2.obj");
int Floor_ver_cnt = Floor_obj.loadObj_normalize_center("Floor.obj");
int jumpbar_ver_cnt = Jumpbar_obj.loadObj_normalize_center("Jumpbar.obj");

vector<glm::vec3> point_col(point_ver_cnt, glm::vec3(1.0, 1.0, 0.0));
vector<glm::vec3> heal_col(heal_ver_cnt, glm::vec3(1.0, 0.0, 0.0));
vector<glm::vec3> shield_col(shield_ver_cnt, glm::vec3(0.0, 0.0, 1.0));
vector<glm::vec3> rock_col(Rock_ver_cnt, glm::vec3(0.5, 0.5, 0.5));
vector<glm::vec3> Bomb_col(Bomb_ver_cnt, glm::vec3(0.0, 0.0, 0.0));
vector<glm::vec3> Thorn_col(Thorn_ver_cnt, glm::vec3(1.0, 0.0, 0.0));
vector<glm::vec3> chara_col(36, glm::vec3(0.0, 1.0, 0.0));
vector<glm::vec3> wall_col(36, glm::vec3(0.92, 0.92, 0.92));
vector<glm::vec3> wall2_col(Wall2_ver_cnt, glm::vec3(0.92, 0.92, 0.92));
vector<glm::vec3> Floor_col(Floor_ver_cnt, glm::vec3(0.96, 0.96, 0.96));
vector<glm::vec3> jumpbar_col(jumpbar_ver_cnt, glm::vec3(0.82, 0.82, 0.82));

vector<glm::vec3> HP;
glm::mat4 S = glm::scale(glm::mat4(1.0), glm::vec3(0.025f, 0.025f, 0.025f));

Map* sansate = new Map();
Map* map = new Map[4];
Map* map2 = new Map[4];
Map* map3 = new Map[4];
Character* ch = new Character();
vector<float> generator;
Rock* rock = new Rock[10];
Bomb* bomb = new Bomb[10];
Thorn* thorn = new Thorn[10];
Point* point = new Point();
Heal* heal = new Heal();
Shield* shield = new Shield();
Jumpbar* jumpbar = new Jumpbar[10];

glm::mat4 Trans;
int gene_count = 0;
float angle = atan(cameraPos.z / cameraPos.y);
int d = 1, jump_cnt = 0;
bool jumping = false;

void Gimic_Keyinput_check(int value);
void Gimic_Spacebar_check(int value);

bool gimic_keyinput = false;
int key_array[8] = { 0 }; //기믹 시작할때 랜덤으로 값 정해주기
int now_array = 0; //배열 중 현 위치
int now_key = 0; //키를 입력받아 값 바꿔준다
clock_t start_time, now_time;//시작 시간

bool gimic_spacebar = false;
int goal_spacebar = 0;//스페이스바 목표치
int now_spacebar = 0;//현재 스페이스바 입력 수

void isdead(int value);

int spe = 30; //겜 속도
void Speedup(int value);
enum Obs {
	ROCK,
	BOMB,
	THORN
};
enum ITEM {
	SCORE,
	HEAL,
	SHIELD
};
void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정
{
	srand((unsigned int)time(NULL));
	//--- 윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 10);
	glutInitWindowSize(600, 750);
	glutCreateWindow("Mini Ski");
	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();
	InitShader();
	InitBuffer();

	map[0].init_Pos(glm::vec3(0.0, 0.0, 0.0), 0.0);
	map[1].init_Pos(glm::vec3(0.0, 0.0, -Floor_obj.scaleZ), 0.0);
	map[2].init_Pos(glm::vec3(0.0, 0.0, -Floor_obj.scaleZ * 2), 0.0);
	map[3].init_Pos(glm::vec3(0.0, 0.0, Z), 0.0);

	map2[0].init_Pos(glm::vec3(-(Floor_obj.scaleX + wall_obj.scaleX) / 2, 0.0, 0.0), 0.0);
	map2[1].init_Pos(glm::vec3(-(Floor_obj.scaleX + wall_obj.scaleX) / 2, 0.0, -Floor_obj.scaleZ), 0.0);
	map2[2].init_Pos(glm::vec3(-(Floor_obj.scaleX + wall_obj.scaleX) / 2, 0.0, -Floor_obj.scaleZ * 2), 0.0);
	map2[3].init_Pos(glm::vec3(-(Floor_obj.scaleX + wall_obj.scaleX) / 2, 0.0, Z), 0.0);

	map3[0].init_Pos(glm::vec3((Floor_obj.scaleX + wall_obj.scaleX) / 2, 0.0, 0.0), 0.0);
	map3[1].init_Pos(glm::vec3((Floor_obj.scaleX + wall_obj.scaleX) / 2, 0.0, -Floor_obj.scaleZ), 0.0);
	map3[2].init_Pos(glm::vec3((Floor_obj.scaleX + wall_obj.scaleX) / 2, 0.0, -Floor_obj.scaleZ * 2), 0.0);
	map3[3].init_Pos(glm::vec3((Floor_obj.scaleX + wall_obj.scaleX) / 2, 0.0, Z), 0.0);

	sansate->init_Pos(glm::vec3(0.0,0.0,9.3),90.0);
	
	for (int i = 0; i < GENERATOR; i++) {
		generator.push_back(-6.0 + 0.5 * i);
	}

	HP.push_back(glm::vec3(-0.36, 3.9, 7.0));
	HP.push_back(glm::vec3(-0.31, 3.9, 7.0));
	HP.push_back(glm::vec3(-0.26, 3.9, 7.0));

	//원근 투영
	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.0f);

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(25, Speedup, 1);
	glutTimerFunc(spe, Timer, 1);
	glutTimerFunc(100, Jump, 1);
	glutTimerFunc(100, isdead, 1);
	glutTimerFunc(50, Collision_check, 1);
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

	//point
	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, Point_obj.outvertex.size() * sizeof(glm::vec3), &Point_obj.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, point_col.size() * sizeof(glm::vec3), &point_col[0][0], GL_STATIC_DRAW);
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

	//heal
	glBindVertexArray(vao[7]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[14]);
	glBufferData(GL_ARRAY_BUFFER, Heal_obj.outvertex.size() * sizeof(glm::vec3), &Heal_obj.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[15]);
	glBufferData(GL_ARRAY_BUFFER, heal_col.size() * sizeof(glm::vec3), &heal_col[0][0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	//shield
	glBindVertexArray(vao[8]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[16]);
	glBufferData(GL_ARRAY_BUFFER, Shield_obj.outvertex.size() * sizeof(glm::vec3), &Shield_obj.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[17]);
	glBufferData(GL_ARRAY_BUFFER, shield_col.size() * sizeof(glm::vec3), &shield_col[0][0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	//wall2
	glBindVertexArray(vao[9]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[18]);
	glBufferData(GL_ARRAY_BUFFER, wall2_obj.outvertex.size() * sizeof(glm::vec3), &wall2_obj.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[19]);
	glBufferData(GL_ARRAY_BUFFER, wall2_col.size() * sizeof(glm::vec3), &wall2_col[0][0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	//jumpbar
	glBindVertexArray(vao[10]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[20]);
	glBufferData(GL_ARRAY_BUFFER, Jumpbar_obj.outvertex.size() * sizeof(glm::vec3), &Jumpbar_obj.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[21]);
	glBufferData(GL_ARRAY_BUFFER, jumpbar_col.size() * sizeof(glm::vec3), &jumpbar_col[0][0], GL_STATIC_DRAW);
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
	//cout << "life : " << ch->hp << endl;
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
	0 point
	1 rock
	2 character
	3 wall
	4 floor
	5 bomb
	6 thorn
	7 heal
	8 shield
	9 wall2
	10 jumpbar
	*/
	glm::mat4 SS = glm::mat4(1.0f);
	SS = glm::scale(SS, glm::vec3(1.0, 2.0, 1.0));
	glm::mat4 SSS = glm::mat4(1.0f);
	SSS = glm::scale(SSS, glm::vec3(2.0, 1.0, 1.0));
	// rock

	glBindVertexArray(vao[7]);
	for (int i = 0; i < ch->hp; i++) {
		Trans = glm::translate(glm::mat4(1.0), HP[i]);
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(Trans * S));
		glDrawArrays(GL_TRIANGLES, 0, heal_ver_cnt);
	}

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

	//jumpbar
	glBindVertexArray(vao[10]);
	for (int i = 0; i < 10; i++) {
		if (jumpbar[i].created) {
			Trans = glm::translate(glm::mat4(1.0), jumpbar[i].T);
			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(Trans));
			glDrawArrays(GL_TRIANGLES, 0, jumpbar_ver_cnt);
		}
	}

	// character
	glBindVertexArray(vao[2]);
	Trans = glm::translate(glm::mat4(1.0), ch->T);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(Trans));
	glDrawArrays(GL_TRIANGLES, 0, Ch_ver_cnt);

	// map
	for (int i = 0; i < 4; i++) {
		glBindVertexArray(vao[9]);
		Trans = glm::translate(glm::mat4(1.0), map2[i].T);
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(Trans*SS));
		glDrawArrays(GL_TRIANGLES, 0, Wall2_ver_cnt);
		Trans = glm::translate(glm::mat4(1.0), map3[i].T);
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(Trans*SS));
		glDrawArrays(GL_TRIANGLES, 0, Wall2_ver_cnt);
		glBindVertexArray(vao[4]);
		Trans = glm::translate(glm::mat4(1.0), map[i].T);
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(Trans*SSS));
		glDrawArrays(GL_TRIANGLES, 0, Floor_ver_cnt);
	}

	//sansate
	if (gimic_spacebar) {
		glBindVertexArray(vao[3]);
		Trans = glm::translate(glm::mat4(1.0), sansate->T);
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(Trans * sansate->R));
		glDrawArrays(GL_TRIANGLES, 0, Wall_ver_cnt);
	}
	// point
	glBindVertexArray(vao[0]);
	if (point->created) {
		Trans = glm::translate(glm::mat4(1.0), point->T);
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(Trans));
		glDrawArrays(GL_TRIANGLES, 0, point_ver_cnt);
	}

	//heal
	glBindVertexArray(vao[7]);
	if (heal->created) {
		Trans = glm::translate(glm::mat4(1.0), heal->T);
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(Trans));
		glDrawArrays(GL_TRIANGLES, 0, heal_ver_cnt);
	}
	//shield
	glBindVertexArray(vao[8]);
	if (shield->created) {
		Trans = glm::translate(glm::mat4(1.0), shield->T);
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(Trans));
		glDrawArrays(GL_TRIANGLES, 0, shield_ver_cnt);
	}

	glutSwapBuffers();
}
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 
{
	glViewport(0, 0, w, h);
}
void Keyboard(unsigned char key, int x, int y)
{
	if (!gimic_keyinput&&!gimic_spacebar) {
		switch (key) {
		case 'd':
			Change_Direction();
			break;
			// space
		case 32:
			if (jumping)
				break;
			jumping = true;
			break;

			//enter
		case 13:
			break;
		}
	}
	else if(gimic_keyinput) {
		switch (key) {
		case 'a':
		case 'A':
			now_key = 65;
			break;
		case 's':
		case 'S':
			now_key = 83;
			break;
		case 'd':
		case 'D':
			now_key = 68;
			break;
		case 'f':
		case 'F':
			now_key = 70;
			break;
		case 'q':
		case 'Q':
			now_key = 81;
			break;
		case 'w':
		case 'W':
			now_key = 87;
			break;
		case 'e':
		case 'E':
			now_key = 69;
			break;
		case 'r':
		case 'R':
			now_key = 82;
			break;
		}
		if (now_key != 0) {//0 이면 입력 받지 않은 상태
			if (key_array[now_array] == now_key) {// 맞았다!
				now_array++;
				for (int i = now_array; i < 8; i++)
					printf("%c", key_array[i]);
				printf("\n");
			}
		}
	}
	else if (gimic_spacebar) {
		switch (key) {
		case 32:
			now_spacebar++;
			printf("%d\n", goal_spacebar - now_spacebar);
			break;
		}
	}
	glutPostRedisplay();
}
void Change_Direction() {
	d *= -1;
	for (int i = 0; i < 4; i++) {
		map[i].dir *= -1;
		map2[i].dir *= -1;
		map3[i].dir *= -1;
	}
	for (int i = 0; i < 10; i++) {
		rock[i].dir *= -1;
		bomb[i].dir *= -1;
		thorn[i].dir *= -1;
		jumpbar[i].dir *= -1;
	}
	point->dir *= -1;
	shield->dir *= -1;
	heal->dir *= -1;
	spe = 40;
}
void isdead(int value) {
	if (ch->hp <= 0) {
		cout << "Point : " << ch->point << "HP : " << ch->hp << "shield : " << ch->has_shield << endl;
		cout << "죽었습니다..." << endl;

		glutTimerFunc(100, isdead, 1);
	}
}
void Speedup(int value) {
	if (spe > 26) {
		spe--;
	}
	
	glutTimerFunc(25, Speedup, 1);
}
void Timer(int value) {
	for (int i = 0; i < 4; i++) {
		map[i].Move();
		map2[i].Move();
		map3[i].Move();
		if (!gimic_keyinput&&!gimic_spacebar) {
			map[i].DirMove();
			map2[i].DirMove();
			map3[i].DirMove();
		}
		
		if (map[i].T.z > cameraPos.z - cameraPos.y * tan((angle - glm::radians(22.5f))) + 10) {
			map[i].T.z = Z;
		}
		if (map2[i].T.z > cameraPos.z - cameraPos.y * tan((angle - glm::radians(22.5f))) + 10) {
			map2[i].T.z = Z;
		}
		if (map3[i].T.z > cameraPos.z - cameraPos.y * tan((angle - glm::radians(22.5f))) + 10) {
			map3[i].T.z = Z;
		}
	}
	
	for (int i = 0; i < 10; i++) {
		if (rock[i].created) {
			rock[i].Move();
			if (!gimic_keyinput && !gimic_spacebar) {
				rock[i].DirMove();
			}
		}

		if (rock[i].T.z > cameraPos.z - cameraPos.y * tan((angle - glm::radians(22.5f))) + 10)
			rock[i].created = false;

		if (bomb[i].created) {
			bomb[i].Move();
			if (!gimic_keyinput && !gimic_spacebar) {
				bomb[i].DirMove();
			}
		}
		if (bomb[i].T.z > cameraPos.z - cameraPos.y * tan((angle - glm::radians(22.5f))) + 10)
			bomb[i].created = false;

		if (thorn[i].created) {
			thorn[i].Move();
			if (!gimic_keyinput && !gimic_spacebar) {
				thorn[i].DirMove();
			}
		}
		if (thorn[i].T.z > cameraPos.z - cameraPos.y * tan((angle - glm::radians(22.5f))) + 10)
			thorn[i].created = false;

		if (jumpbar[i].created) {
			jumpbar[i].Move();
			if (!gimic_keyinput && !gimic_spacebar) {
				jumpbar[i].DirMove();
			}
		}
		if (jumpbar[i].T.z > cameraPos.z - cameraPos.y * tan((angle - glm::radians(22.5f))) + 10)
			jumpbar[i].created = false;
	}

	if (point->created) {
		point->Move();
		if (!gimic_keyinput && !gimic_spacebar) {
			point->DirMove();
		}
	}
	if (point->T.z > cameraPos.z - cameraPos.y * tan((angle - glm::radians(22.5f))) + 10)
		point->created = false;

	
	if (heal->created) {
		heal->Move();
		if (!gimic_keyinput && !gimic_spacebar) {
			heal->DirMove();
		}
	}

	if (heal->T.z > cameraPos.z - cameraPos.y * tan((angle - glm::radians(22.5f))) + 10)
		heal->created = false;

	if (shield->created) {
		shield->Move();
		if (!gimic_keyinput && !gimic_spacebar) {
			shield->DirMove();
		}
	}

	if (shield->T.z > cameraPos.z - cameraPos.y * tan((angle - glm::radians(22.5f))) + 10)
		shield->created = false;

	if (!gimic_keyinput && !gimic_spacebar) {
		if (d == 1)
			for (int i = 0; i < GENERATOR; i++) {
				generator[i] -= 0.1;
			}
		else
			for (int i = 0; i < GENERATOR; i++) {
				generator[i] += 0.1;
			}
	}

	glutTimerFunc(spe, Timer, 1);
	glutPostRedisplay();
}
void Jump(int value) {
	if (!gimic_keyinput) {
		if (jumping) {
			if (jump_cnt > 15) {
				jumping = 0;
				jump_cnt = 0;
			}
			else if (jump_cnt > 7) {
				ch->jump(-0.1);
				jump_cnt++;
			}
			else {
				ch->jump(0.1);
				jump_cnt++;
			}
		}
	}
	else {
		if(jumping)
			if (jump_cnt <= 7) {
				ch->jump(0.1);
				jump_cnt++;
			}
	}
	glutTimerFunc(100, Jump, 1);
	glutPostRedisplay();
}
void generateOb(int value) {

	int kinds = rand() % 3;
	int idx = rand() % 25;
	int sansate;
	if (!gimic_spacebar&&!gimic_keyinput) {
		sansate = rand() % 10;

		if (sansate == 0) {
			now_spacebar = 0;
			goal_spacebar = rand() % 5 + 40;
			gimic_spacebar = true;

			start_time = clock();
			printf("space bar를 연타해주세요!!!\n");
		
			printf("%d", goal_spacebar);
			printf("\n");

			glutTimerFunc(25, Gimic_Spacebar_check, 1);
		}
	}
	if (gene_count % 5 != 0&&gene_count%6!=0) {
		switch (kinds) {
		case Obs::ROCK:
			for (int i = 0; i < 10; i++) {
				if (!rock[i].created) {
					rock[i].init_pos(glm::vec3(generator[idx], 0.0, Z), 0.0);
					rock[i].created = true;
					break;
				}
			}
			break;
		case Obs::BOMB:
			for (int i = 0; i < 10; i++) {
				if (!bomb[i].created) {
					bomb[i].init_pos(glm::vec3(generator[idx], 0.0, Z), 0.0);
					bomb[i].created = true;
					break;
				}
			}
			break;
		case Obs::THORN:
			for (int i = 0; i < 10; i++) {
				if (!thorn[i].created) {
					thorn[i].init_pos(glm::vec3(generator[idx], 0.0, Z), 0.0);
					thorn[i].created = true;
					break;
				}
			}
			break;
		}
	}
	else if (gene_count %6 ==0) {
		for (int i = 0; i < 10; i++) {
			if (!jumpbar[i].created) {
				jumpbar[i].init_pos(glm::vec3(generator[idx], 0.0, Z), 0.0);
				jumpbar[i].created = true;
				break;
			}
		}
	}
	else if(gene_count %5==0) {
		switch (kinds) {
		case ITEM::SCORE:
			if (!point->created) {
				point->init_pos(glm::vec3(generator[idx], 0.0, Z), 0.0);
				point->created = true;
			}
			break;
		case ITEM::HEAL:
			if (!heal->created) {
				heal->init_pos(glm::vec3(generator[idx], 0.0, Z), 0.0);
				heal->created = true;
			}
			break;
		case ITEM::SHIELD:
			if (!shield->created) {
				shield->init_pos(glm::vec3(generator[idx], 0.0, Z), 0.0);
				shield->created = true;
			}
			break;
		}
	}
	gene_count++;
	// 3초당 생성
	glutTimerFunc(3000, generateOb, 1);
}
void Gimic_Keyinput_check(int value) {
	if (gimic_keyinput) {//기믹 온 상태
		now_time = clock();
		if ((double)(now_time - start_time) / CLOCKS_PER_SEC >= 10.0f) { //시간이 10.0초가 지났다. 기믹 실패!
			//체력 감소
			ch->hp -= 1;
			system("cls");
			printf("Fail...\n");
			cout << "Point : " << ch->point << "HP : " << ch->hp << "shield : " << ch->has_shield << endl;
			gimic_keyinput = false;
			return;
		}

		if (now_array > 7) {//기믹 클리어!
			ch->point += 500;
			system("cls");
			printf("GREAT!!!\n");
			cout << "Point : " << ch->point << "HP : " << ch->hp << "shield : " << ch->has_shield << endl;
			gimic_keyinput = false;//false시켜서 타이머 종료
		}
		
		now_key = 0;
		glutTimerFunc(25, Gimic_Keyinput_check, 1);
	}
}
void Gimic_Spacebar_check(int value) {
	if (gimic_spacebar) {
		now_time = clock();
		if ((double)(now_time - start_time) / CLOCKS_PER_SEC >= 10.0f) { //시간이 10.0초가 지났다. 기믹 실패!
			//체력 감소
			//즉사!
			if (ch->hp > 0)
				ch->hp = 0;
			system("cls");
			printf("Fail...\n");
			cout << "Point : " << ch->point << "HP : " << ch->hp << "shield : " << ch->has_shield << endl;

			gimic_spacebar = false;
			return;
		}

		if (now_spacebar >= goal_spacebar) { //기믹 성공!
			ch->point += 500;
			system("cls");
			printf("Great!!!!\n");
			cout << "Point : " << ch->point << "HP : " << ch->hp << "shield : " << ch->has_shield << endl;

			gimic_spacebar = false;//false시켜서 타이머 종료
		}

		glutTimerFunc(25, Gimic_Spacebar_check, 1);
	}
}
bool wall_collid(int type, glm::vec3 v) {
	glm::mat4 TT = glm::mat4(1.0f);
	TT = glm::translate(TT, v);
	glm::vec3 now = glm::vec3(0.0, 0.0, 0.0);

	switch (type) {
	case 0:
		for (int i = 0; i < wall2_obj.outvertex.size(); i++) {
			now = wall2_obj.outvertex[i];
			now = TT * glm::vec4(now, 1.0f);
			if ((now.x >= ch->T.x - chara_obj.scaleX / 2)
				&& (now.z <= ch->T.z + chara_obj.scaleZ / 2
					&& now.z >= ch->T.z - chara_obj.scaleZ / 2)
				)
				return true;
		}

		/*if (ch->T.x - chara_obj.scaleX / 2 < v.x + wall_obj.scaleX / 2)
			return true;*/
		break;
	case 1:
		for (int i = 0; i < wall2_obj.outvertex.size(); i++) {
			now = wall2_obj.outvertex[i];
			now = TT * glm::vec4(now, 1.0f);
			if ((now.x <= ch->T.x + chara_obj.scaleX / 2)
				&& (now.z <= ch->T.z + chara_obj.scaleZ / 2
					&& now.z >= ch->T.z - chara_obj.scaleZ / 2)
				)
				return true;
		}
		/*if (ch->T.x + chara_obj.scaleX / 2 > v.x - wall_obj.scaleX / 2)
			return true;*/
		break;
	}
	return false;
}
bool chara_collision_cal(glm::vec3 a, float scaleX, float scaleY, float scaleZ) {//점과 캐릭터의 모든 평면과 체크
	if (((ch->T.x + chara_obj.scaleX / 2 >= a.x - scaleX
		&& ch->T.x + chara_obj.scaleX / 2 <= a.x + scaleX)
		|| (ch->T.x - chara_obj.scaleX / 2 >= a.x - scaleX
			&& ch->T.x - chara_obj.scaleX / 2 <= a.x + scaleX)
		|| (a.x >= ch->T.x - chara_obj.scaleX/2
			&& a.x<= ch->T.x+chara_obj.scaleX/2))
		&& ((ch->T.y >= a.y && ch->T.y <= a.y + scaleY))
		&& ((ch->T.z + chara_obj.scaleZ / 2 >= a.z - scaleZ
			&& ch->T.z + chara_obj.scaleZ / 2 <= a.z + scaleZ)
			|| (ch->T.z - chara_obj.scaleZ / 2 >= a.z - scaleZ
				&& ch->T.z - chara_obj.scaleZ / 2 <= a.z + scaleZ)
			||(a.z>= ch->T.z-chara_obj.scaleZ
				&&a.z<=ch->T.z+chara_obj.scaleZ)))
		return true;
	return false;
}
void Collision_check(int value) {
	if (!gimic_keyinput&&!gimic_spacebar) {
		for (int k = 0; k < 4; k++) {
			if (wall_collid(0, map2[k].T)) {
				ch->hp--;
				system("cls");
				cout << "Point : " << ch->point << "HP : " << ch->hp << "shield : " << ch->has_shield << endl;
				for (int i = 0; i < 3; i++)
					HP[i].x += 2.;
				Change_Direction();
				ch->T.x += 2.;
				cameraPos.x += 2.;
				cameraDirection.x += 2.;
				for (int i = 0; i < 4; i++) {
					map[i].T.z -= 1.;
					map2[i].T.z -= 1.;
					map3[i].T.z -= 1.;
				}
				for (int i = 0; i < 10; i++) {
					rock[i].T.z -= 1.;
					bomb[i].T.z -= 1.;
					thorn[i].T.z -= 1.;
					jumpbar[i].T.z -= 1.;
				}
				point->T.z -= 1.;
				shield->T.z -= 1.;
				heal->T.z -= 1.;
			}//map2 와 충돌

			if (wall_collid(1, map3[k].T)) {
				ch->hp--;
				system("cls");
				cout << "Point : " << ch->point << "HP : " << ch->hp << "shield : " << ch->has_shield << endl;
				for (int i = 0; i < 3; i++)
					HP[i].x -= 2.;
				Change_Direction();
				ch->T.x -= 2.;
				cameraPos.x -= 2.;
				cameraDirection.x -= 2.0;
				for (int i = 0; i < 4; i++) {
					map[i].T.z -= 1.;
					map2[i].T.z -= 1.;
					map3[i].T.z -= 1.;
				}
				for (int i = 0; i < 10; i++) {
					rock[i].T.z -= 1.;
					bomb[i].T.z -= 1.;
					thorn[i].T.z -= 1.;
					jumpbar[i].T.z -= 1;
				}
				point->T.z -= 1.;
				shield->T.z -= 1.;
				heal->T.z -= 1.;
			}//map3 와 충돌
		}


		for (int i = 0; i < 10; i++) {
			if (rock[i].created) {
				if (chara_collision_cal(rock[i].T, Rock_obj.scaleX / 2, Rock_obj.scaleY / 2, Rock_obj.scaleZ / 2)) {
					if (!ch->has_shield)
						ch->hp--;
					else ch->has_shield = false;
					Change_Direction();
					system("cls");
					cout << "Point : " << ch->point << "HP : " << ch->hp << "shield : " << ch->has_shield << endl;

					rock[i].created = false;
				}
			}
			if (bomb[i].created) {
				if (chara_collision_cal(bomb[i].T, Bomb_obj.scaleX / 2, Bomb_obj.scaleY / 2, Bomb_obj.scaleZ / 2)) {
					if (!ch->has_shield)
						ch->hp--;
					else ch->has_shield = false;
					Change_Direction();
					system("cls");
					cout << "Point : " << ch->point << "HP : " << ch->hp << "shield : " << ch->has_shield << endl;
				
					if (ch->T.x >= bomb[i].T.x) {
						for (int i = 0; i < 3; i++)
							HP[i].x += 1.;
						ch->T.x += 1.;
						cameraPos.x += 1.;
						cameraDirection.x += 1.;
					}
					else {
						for (int i = 0; i < 3; i++)
							HP[i].x -= 1.;
						ch->T.x -= 1.;
						cameraPos.x -= 1.;
						cameraDirection.x -= 1.;
					}
					bomb[i].created = false;
				}
			}
			if (thorn[i].created) {
				if (chara_collision_cal(thorn[i].T, Thorn_obj.scaleX / 2, Thorn_obj.scaleY / 2, Thorn_obj.scaleZ / 2)) {
					if (!ch->has_shield)
						ch->hp--;
					else ch->has_shield = false;
					system("cls");
					cout << "Point : " << ch->point << "HP : " << ch->hp << "shield : " << ch->has_shield << endl;

					thorn[i].created = false;
				}
			}
			if (jumpbar[i].created) {
				if (chara_collision_cal(jumpbar[i].T, Jumpbar_obj.scaleX / 2, Jumpbar_obj.scaleY / 2, Jumpbar_obj.scaleZ / 2)) {
					if (jump_cnt <= 4 && jump_cnt >= 1) {
						gimic_keyinput = true;
						start_time = clock();
						now_array = 0;
						initkeyarray();
						printf("아래 키 배열을 입력해주세요!!!\n");
						for (int i = now_array; i < 8; i++)
							printf("%c", key_array[i]);
						printf("\n");
						glutTimerFunc(25, Gimic_Keyinput_check, 1);
						jumpbar[i].created = false;
					}
				}
			}
		}
		if (point->created) {
			if (chara_collision_cal(point->T, Point_obj.scaleX, Point_obj.scaleY, Point_obj.scaleZ)) {
				ch->point += 100;
				point->created = false;
				system("cls");
				cout << "Point : " << ch->point << "HP : " << ch->hp << "shield : " << ch->has_shield << endl;
			}
		}
		if (heal->created) {
			if (chara_collision_cal(heal->T, Heal_obj.scaleX, Heal_obj.scaleY, Heal_obj.scaleZ)) {
				if (ch->hp < 3)
					ch->hp += 1;
				system("cls");
				cout << "Point : " << ch->point << "HP : " << ch->hp << "shield : " << ch->has_shield << endl;
				heal->created = false;
			}
		}
		if (shield->created) {
			if (chara_collision_cal(shield->T, Shield_obj.scaleX, Shield_obj.scaleY, Shield_obj.scaleZ)) {
				if (!ch->has_shield)
					ch->has_shield = true;
				system("cls");
				cout << "Point : " << ch->point << "HP : " << ch->hp << "shield : " << ch->has_shield << endl;
				shield->created = false;
			}
		}
	}

	glutTimerFunc(50, Collision_check, 1);
	glutPostRedisplay();
}

void initkeyarray() {
	for (int i = 0; i < 8; i++) {
		key_array[i] = rand() % 8;
		switch (key_array[i]) {
		case 0:
			key_array[i] = 65;
			break;
		case 1:
			key_array[i] = 83;
			break;
		case 2:
			key_array[i] = 68;
			break;
		case 3:
			key_array[i] = 70;
			break;
		case 4:
			key_array[i] = 81;
			break;
		case 5:
			key_array[i] = 87;
			break;
		case 6:
			key_array[i] = 69;
			break;
		case 7:
			key_array[i] = 82;
			break;
		}
	}
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