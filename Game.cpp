#include "objRead.h"

#define VAO_SIZE 20
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
void Char_Rotate(int value);
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
objRead Floor_obj;

int point_ver_cnt = Point_obj.loadObj_normalize_center("Cube.obj");
int heal_ver_cnt = Heal_obj.loadObj_normalize_center("Cube.obj");
int shield_ver_cnt = Shield_obj.loadObj_normalize_center("Cube.obj");
int Bomb_ver_cnt = Bomb_obj.loadObj_normalize_center("Cube.obj");
int Rock_ver_cnt = Rock_obj.loadObj_normalize_center("Rock.obj");
int Thorn_ver_cnt = Thorn_obj.loadObj_normalize_center("Thorn.obj");
int Ch_ver_cnt = chara_obj.loadObj_normalize_center("Char.obj");
int Wall_ver_cnt = wall_obj.loadObj_normalize_center("Wall.obj");
int Floor_ver_cnt = Floor_obj.loadObj_normalize_center("Floor.obj");

vector<glm::vec3> point_col(36, glm::vec3(0.0, 0.0, 1.0));
vector<glm::vec3> heal_col(36, glm::vec3(0.0, 0.0, 1.0));
vector<glm::vec3> shield_col(36, glm::vec3(0.0, 0.0, 1.0));
vector<glm::vec3> rock_col(Rock_ver_cnt, glm::vec3(0.5, 0.5, 0.5));
vector<glm::vec3> Bomb_col(36, glm::vec3(0.0, 0.0, 0.0));
vector<glm::vec3> Thorn_col(Thorn_ver_cnt, glm::vec3(1.0, 0.0, 0.0));
vector<glm::vec3> chara_col(36, glm::vec3(0.0, 1.0, 0.0));
vector<glm::vec3> wall_col(36, glm::vec3(0.92, 0.92, 0.92));
vector<glm::vec3> Floor_col(Floor_ver_cnt, glm::vec3(0.96, 0.96, 0.96));

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

glm::mat4 Trans;
float angle = atan(cameraPos.z / cameraPos.y);
int d = 1, jump_cnt = 0;
bool jumping = false;
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
void main(int argc, char** argv) //--- ?????? ???????? ???????? ????
{
	srand((unsigned int)time(NULL));
	//--- ?????? ????????
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 10);
	glutInitWindowSize(600, 750);
	glutCreateWindow("Mini Ski");
	//--- GLEW ??????????
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
	
	for (int i = 0; i < GENERATOR; i++) {
		generator.push_back(-6.0 + 0.5 * i);
	}

	//???? ????
	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.0f);

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(50, Timer, 1);
	glutTimerFunc(50, Char_Rotate, 1);
	glutTimerFunc(100, Jump, 1);
	glutTimerFunc(50, Collision_check, 1);
	glutTimerFunc(5000, generateOb, 1);
	glutMainLoop();
}
void make_vertexShader()
{
	vertexsource = filetobuf("vertex.glsl");
	//--- ?????? ?????? ???? ??????
	vertexshader = glCreateShader(GL_VERTEX_SHADER);
	//--- ?????? ?????? ?????? ?????? ????
	glShaderSource(vertexshader, 1, (const GLchar**)&vertexsource, 0);
	//--- ?????? ?????? ??????????
	glCompileShader(vertexshader);
	//--- ???????? ?????? ???? ???? ????: ???? ????
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexshader, 512, NULL, errorLog);
		cerr << "ERROR: vertex shader ?????? ????\n" << errorLog << endl;
		return;
	}
}
void make_fragmentShader()
{
	fragmentsource = filetobuf("fragment.glsl");
	//--- ?????????? ?????? ???? ??????
	fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
	//--- ?????? ?????? ?????? ?????? ????
	glShaderSource(fragmentshader, 1, (const GLchar**)&fragmentsource, 0);
	//--- ?????????? ?????? ??????
	glCompileShader(fragmentshader);
	//--- ???????? ?????? ???? ???? ????: ?????? ???? ????
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentshader, 512, NULL, errorLog);
		cerr << "ERROR: fragment shader ?????? ????\n" << errorLog << endl;
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
}
void InitShader()
{
	make_vertexShader(); //--- ?????? ?????? ??????
	make_fragmentShader(); //--- ?????????? ?????? ??????
	//-- shader Program
	s_program = glCreateProgram();
	glAttachShader(s_program, vertexshader);
	glAttachShader(s_program, fragmentshader);
	glLinkProgram(s_program);
	//--- ?????? ????????
	glDeleteShader(vertexshader);
	glDeleteShader(fragmentshader);
	//--- Shader Program ????????
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
	*/

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
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(Trans * ch->R));
	glDrawArrays(GL_TRIANGLES, 0, Ch_ver_cnt);

	// map
	for (int i = 0; i < 4; i++) {
		glBindVertexArray(vao[3]);
		Trans = glm::translate(glm::mat4(1.0), map2[i].T);
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(Trans));
		glDrawArrays(GL_TRIANGLES, 0, Wall_ver_cnt);
		Trans = glm::translate(glm::mat4(1.0), map3[i].T);
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(Trans));
		glDrawArrays(GL_TRIANGLES, 0, Wall_ver_cnt);
		glBindVertexArray(vao[4]);
		Trans = glm::translate(glm::mat4(1.0), map[i].T);
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(Trans));
		glDrawArrays(GL_TRIANGLES, 0, Floor_ver_cnt);
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
GLvoid Reshape(int w, int h) //--- ???? ????: ???? ?????? ???? ???? 
{
	glViewport(0, 0, w, h);
}
void Keyboard(unsigned char key, int x, int y)
{
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
	glutPostRedisplay();
}
void Change_Direction() {
	d *= -1;
	for (int i = 0; i < 4; i++) {
		map[i].dir = d;
		map2[i].dir = d;
		map3[i].dir = d;
	}
	for (int i = 0; i < 10; i++) {
		rock[i].dir = d;
		bomb[i].dir = d;
		thorn[i].dir = d;
	}
	point->dir = d;
	shield->dir = d;
	heal->dir = d;
}
void Char_Rotate(int value) {

}
void Timer(int value) {
	for (int i = 0; i < 4; i++) {
		map[i].Move();
		map2[i].Move();
		map3[i].Move();
		
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
		if (rock[i].created)
			rock[i].Move();

		if (rock[i].T.z > cameraPos.z - cameraPos.y * tan((angle - glm::radians(22.5f))) + 10)
			rock[i].created = false;

		if (bomb[i].created)
			bomb[i].Move();

		if (bomb[i].T.z > cameraPos.z - cameraPos.y * tan((angle - glm::radians(22.5f))) + 10)
			bomb[i].created = false;

		if (thorn[i].created)
			thorn[i].Move();

		if (thorn[i].T.z > cameraPos.z - cameraPos.y * tan((angle - glm::radians(22.5f))) + 10)
			thorn[i].created = false;
	}

	if (point->created)
		point->Move();

	if (point->T.z > cameraPos.z - cameraPos.y * tan((angle - glm::radians(22.5f))) + 10)
		point->T.z = Z;

	if (heal->created)
		heal->Move();

	if (heal->T.z > cameraPos.z - cameraPos.y * tan((angle - glm::radians(22.5f))) + 10)
		heal->T.z = Z;

	if (shield->created)
		shield->Move();

	if (shield->T.z > cameraPos.z - cameraPos.y * tan((angle - glm::radians(22.5f))) + 10)
		shield->T.z = Z;

	if (d == 1)
		for (int i = 0; i < GENERATOR; i++) {
			generator[i] -= 0.1;
		}
	else
		for (int i = 0; i < GENERATOR; i++) {
			generator[i] += 0.1;
		}

	glutTimerFunc(50, Timer, 1);
	glutPostRedisplay();
}
void Jump(int value) {
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
	glutTimerFunc(100, Jump, 1);
	glutPostRedisplay();
}
int gene_count = 0;//???? ???? 5?????? ?????? ????
void generateOb(int value) {

	int kinds = rand() % 3;
	int idx = rand() % 25;
	if (gene_count % 5 != 0) {
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
	else {
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
	// 5???? ????
	glutTimerFunc(5000, generateOb, 1);
}
bool wall_collid(int type, glm::vec3 v) {
	switch (type) {
	case 0:
		if (ch->T.x - chara_obj.scaleX / 2 < v.x + wall_obj.scaleX / 2)
			return true;
		break;
	case 1:
		if (ch->T.x + chara_obj.scaleX / 2 > v.x - wall_obj.scaleX / 2)
			return true;
		break;
	}
	return false;
}
bool chara_collision_cal(glm::vec3 a, float scaleX, float scaleY, float scaleZ) {//???? ???????? ???? ?????? ????
	if (((ch->T.x + chara_obj.scaleX / 2 >= a.x - scaleX
		&& ch->T.x + chara_obj.scaleX / 2 <= a.x + scaleX)
		|| (ch->T.x - chara_obj.scaleX / 2 >= a.x - scaleX
			&& ch->T.x - chara_obj.scaleX / 2 <= a.x + scaleX))
		&& ((ch->T.y >= a.y && ch->T.y <= a.y + scaleY))
		&& ((ch->T.z + chara_obj.scaleZ / 2 >= a.z - scaleZ
			&& ch->T.z + chara_obj.scaleZ / 2 <= a.z + scaleZ)
			|| (ch->T.z - chara_obj.scaleZ / 2 >= a.z - scaleZ
				&& ch->T.z - chara_obj.scaleZ / 2 <= a.z + scaleZ)))
		return true;
	/*if ((ch->T.x + chara_obj.scaleX / 2 >= a.x - scaleX
		&& ch->T.x + chara_obj.scaleX / 2 <= a.x + scaleX)
		|| (ch->T.x - chara_obj.scaleX / 2 >= a.x - scaleX
			&& ch->T.x - chara_obj.scaleX / 2 <= a.x + scaleX))
		cout << "1    ";
		if ((ch->T.y >= a.y && ch->T.y <= a.y + scaleY))
		cout << "2    ";
		if ((ch->T.z + chara_obj.scaleZ / 2 >= a.z - scaleZ
			&& ch->T.z + chara_obj.scaleZ / 2 <= a.z + scaleZ)
			|| (ch->T.z - chara_obj.scaleZ / 2 >= a.z - scaleZ
				&& ch->T.z - chara_obj.scaleZ / 2 <= a.z + scaleZ))
		cout << "3    ";

		cout << endl;*/
	return false;
}
void Collision_check(int value) {
	if (wall_collid(0, map2->T)) {
		ch->hp--;
		cout << d << endl;
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

		}
		point->dir *= -1;
		shield->dir *= -1;
		heal->dir *= -1;
	}//map2 ?? ????
	if (wall_collid(1, map3->T)) {
		ch->hp--;
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
			
		}
		point->dir *= -1;
		shield->dir *= -1;
		heal->dir *= -1;
	}//map3 ?? ????

	for (int i = 0; i < 10; i++) {
		if (rock[i].created) {
			if (chara_collision_cal(rock[i].T, Rock_obj.scaleX / 2, Rock_obj.scaleY / 2, Rock_obj.scaleZ / 2)) {
				ch->hp--;
				d *= -1;
				for (int i = 0; i < 3; i++) {
					map[i].dir *= -1;
					map2[i].dir *= -1;
					map3[i].dir *= -1;
				}
				for (int i = 0; i < 10; i++) {
					rock[i].dir *= -1;
					bomb[i].dir *= -1;
					thorn[i].dir *= -1;
					
				}
				point->dir *= -1;;
				shield->dir *= -1;
				heal->dir *= -1;

				rock[i].created = false;
			}
		}
		if (bomb[i].created) {
			if (chara_collision_cal(bomb[i].T, Bomb_obj.scaleX / 2, Bomb_obj.scaleY / 2, Bomb_obj.scaleZ / 2)) {
				ch->hp--;
				d *= -1;
				for (int i = 0; i < 3; i++) {
					map[i].dir *= -1;
					map2[i].dir *= -1;
					map3[i].dir *= -1;
				}
				for (int i = 0; i < 10; i++) {
					rock[i].dir *= -1;
					bomb[i].dir *= -1;
					thorn[i].dir *= -1;

				}
				point->dir *= -1;;
				shield->dir *= -1;
				heal->dir *= -1;
				bomb[i].created = false;
			}
		}
		if (thorn[i].created) {
			if (chara_collision_cal(thorn[i].T, Thorn_obj.scaleX / 2, Thorn_obj.scaleY / 2, Thorn_obj.scaleZ / 2)) {
				ch->hp--;
				thorn[i].created = false;
			}
		}
	}
	//if (point->use) {
	//	if (chara_collision_cal(point->T, Point_obj.scaleX, Point_obj.scaleY, Point_obj.scaleZ)) {
	//		ch->point += 100;
	//		point->use = false;
	//	}
	//}
	//if (heal->use) {
	//	if (chara_collision_cal(heal->T, Heal_obj.scaleX, Heal_obj.scaleY, Heal_obj.scaleZ)) {
	//		if (ch->hp < 3)
	//			ch->hp += 1;
	//		heal->use = false;
	//	}
	//}
	//if (shield->use) {
	//	if (chara_collision_cal(shield->T, Shield_obj.scaleX, Shield_obj.scaleY, Shield_obj.scaleZ)) {
	//		if (ch->shield < 1)
	//			ch->shield += 1;
	//		shield->use = false;
	//	}
	//}

	glutTimerFunc(50, Collision_check, 1);
	glutPostRedisplay();
}
//opengl ?????? ????
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