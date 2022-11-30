#include "objRead.h"

#define VAO_SIZE 5
#define VBO_SIZE 10
#define ROCK 25

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
glm::vec3 cameraPos = glm::vec3(0.0f, 10.0f, 10.0f);
glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); 
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 projection = glm::mat4(1.0f);
unsigned int projectionLocation;
unsigned int viewLocation;
objRead item;
objRead ob;
objRead chara;
objRead wall;
objRead Floor;
int object = item.loadObj_normalize_center("Cube.obj");
int object2 = ob.loadObj_normalize_center("Cube.obj");
int object3 = chara.loadObj_normalize_center("Char.obj");
int object4 = wall.loadObj_normalize_center("Wall.obj");
int object5 = Floor.loadObj_normalize_center("Floor.obj");

vector<glm::vec3> item_col(36, glm::vec3(0.0, 0.0, 1.0));
vector<glm::vec3> ob_col(36, glm::vec3(1.0, 0.0, 0.0));
vector<glm::vec3> chara_col(36, glm::vec3(0.0, 1.0, 0.0));
vector<glm::vec3> wall_col(36, glm::vec3(0.92, 0.92, 0.92));
vector<glm::vec3> Floor_col(object5, glm::vec3(0.96, 0.96, 0.96));

Map* map = new Map[3];
Map* map2 = new Map[3];
Map* map3 = new Map[3];
Character* ch = new Character();
Rock* rock = new Rock[ROCK];
vector<bool> is_Rock(ROCK, false);

int d = 1;
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

	map[1].init_Pos(glm::vec3(-(Floor.scaleX + wall.scaleX) / 2, 0.0, 0.0), 0.0, 0.0);
	map[2].init_Pos(glm::vec3((Floor.scaleX + wall.scaleX) / 2, 0.0, 0.0), 0.0, 0.0);

	map2[0].init_Pos(glm::vec3(0.0, 0.0, -Floor.scaleZ), 0.0, -Floor.scaleZ);
	map2[1].init_Pos(glm::vec3(-(Floor.scaleX + wall.scaleX) / 2, 0.0, -Floor.scaleZ), 0.0, -Floor.scaleZ);
	map2[2].init_Pos(glm::vec3((Floor.scaleX + wall.scaleX) / 2, 0.0, -Floor.scaleZ), 0.0, -Floor.scaleZ);

	map3[0].init_Pos(glm::vec3(0.0, 0.0, -Floor.scaleZ * 2), 0.0, -Floor.scaleZ * 2);
	map3[1].init_Pos(glm::vec3(-(Floor.scaleX + wall.scaleX) / 2, 0.0, -Floor.scaleZ * 2), 0.0,-Floor.scaleZ * 2);
	map3[2].init_Pos(glm::vec3((Floor.scaleX + wall.scaleX) / 2, 0.0, -Floor.scaleZ * 2), 0.0, -Floor.scaleZ * 2);
	
	for (int i = 0; i < ROCK; i++) {
		rock[i].init_pos(-6.0 + 0.5 * i, -Floor.scaleZ * 2, 0.0);
	}

	//원근 투영
	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.0f);

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(50, Timer, 1);
	glutTimerFunc(50, generateOb, 1);
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
	glBufferData(GL_ARRAY_BUFFER, item.outvertex.size() * sizeof(glm::vec3), &item.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, item_col.size() * sizeof(glm::vec3), &item_col[0][0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	//ob
	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, ob.outvertex.size() * sizeof(glm::vec3), &ob.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, ob_col.size() * sizeof(glm::vec3), &ob_col[0][0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	//char
	glBindVertexArray(vao[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glBufferData(GL_ARRAY_BUFFER, chara.outvertex.size() * sizeof(glm::vec3), &chara.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
	glBufferData(GL_ARRAY_BUFFER, chara_col.size() * sizeof(glm::vec3), &chara_col[0][0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	//wall
	glBindVertexArray(vao[3]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);
	glBufferData(GL_ARRAY_BUFFER, wall.outvertex.size() * sizeof(glm::vec3), &wall.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[7]);
	glBufferData(GL_ARRAY_BUFFER, wall_col.size() * sizeof(glm::vec3), &wall_col[0][0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(1);

	//floor
	glBindVertexArray(vao[4]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[8]);
	glBufferData(GL_ARRAY_BUFFER, Floor.outvertex.size() * sizeof(glm::vec3), &Floor.outvertex[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[9]);
	glBufferData(GL_ARRAY_BUFFER, Floor_col.size() * sizeof(glm::vec3), &Floor_col[0][0], GL_STATIC_DRAW);
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

	// item
	glBindVertexArray(vao[0]);


	// ob
	glBindVertexArray(vao[1]);
	for (int i = 0; i < ROCK; i++) {
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(rock[i].T));
		glDrawArrays(GL_TRIANGLES, 0, object2);
	}

	// character
	glBindVertexArray(vao[2]);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(ch->T * ch->R));
	glDrawArrays(GL_TRIANGLES, 0, object3);

	// map
	glBindVertexArray(vao[3]);
	for (int i = 1; i < 3; i++) {
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(map[i].T * map[i].R));
		glDrawArrays(GL_TRIANGLES, 0, object4);
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(map2[i].T * map2[i].R));
		glDrawArrays(GL_TRIANGLES, 0, object4);
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(map3[i].T * map3[i].R));
		glDrawArrays(GL_TRIANGLES, 0, object4);
	}

	glBindVertexArray(vao[4]);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(map[0].T * map[0].R));
	glDrawArrays(GL_TRIANGLES, 0, object5);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(map2[0].T * map2[0].R));
	glDrawArrays(GL_TRIANGLES, 0, object5);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(map3[0].T * map3[0].R));
	glDrawArrays(GL_TRIANGLES, 0, object5);

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
		for (int i = 0; i < ROCK; i++)
			rock[i].dir = d;
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

	map[0].Pos_z += map[0].speed;
	map2[0].Pos_z += map[0].speed;
	map3[0].Pos_z += map[0].speed;
	
	if (map[0].Pos_z >= 16.0) {
		for (int i = 0; i < 3; i++)
			map[i].T = glm::translate(map[i].T, glm::vec3(0.0, 0.0, -Floor.scaleZ * 2));
		map[0].Pos_z += -Floor.scaleZ * 2;
	}
	if (map2[0].Pos_z >= 16.0) {
		for (int i = 0; i < 3; i++)
			map2[i].T = glm::translate(map2[i].T, glm::vec3(0.0, 0.0, -Floor.scaleZ * 2));
		map2[0].Pos_z += -Floor.scaleZ * 2;
	}
	if (map3[0].Pos_z >= 16.0) {
		for (int i = 0; i < 3; i++)
			map3[i].T = glm::translate(map3[i].T, glm::vec3(0.0, 0.0, -Floor.scaleZ * 2));
		map3[0].Pos_z += -Floor.scaleZ * 2;
	}

	for (int i = 0; i < ROCK; i++) {
		rock[i].Move();
	}
	glutTimerFunc(100, Timer, 1);
	glutPostRedisplay();
}
void generageOb(int value) {

	glutTimerFunc(100, generateOb, 1);
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