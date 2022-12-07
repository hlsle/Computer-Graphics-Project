#pragma once
#include "objRead.h"

class Rock  {
public:
	glm::vec3 T = glm::vec3(0.0, 0.0, 0.0);
	glm::mat4 R = glm::mat4(1.0);
	bool created = false;
	int dir = 1;
	float speed = 0.5;

	Rock() {}
	void init_pos(glm::vec3 Pos, float angle) {
		created = true;
		T = Pos;
		R = glm::rotate(glm::mat4(1.0), glm::radians(-angle), glm::vec3(1.0, 0.0, 0.0));
	}

	void Move();
	~Rock(){}
};

class Bomb{
public:
	glm::vec3 T = glm::vec3(0.0, 0.0, 0.0);
	glm::mat4 R = glm::mat4(1.0);
	bool created = false;
	int dir = 1;
	float speed = 0.5;

	Bomb() {}
	void init_pos(glm::vec3 Pos, float angle) {
		created = true;
		T = Pos;
		R = glm::rotate(glm::mat4(1.0), glm::radians(-angle), glm::vec3(1.0, 0.0, 0.0));
	}

	void Move();
	~Bomb(){}
};

class  Thorn {
public:
	glm::vec3 T = glm::vec3(0.0, 0.0, 0.0);
	glm::mat4 R = glm::mat4(1.0);
	bool created = false;
	int dir = 1;
	float speed = 0.5;

	Thorn() {}
	void init_pos(glm::vec3 Pos, float angle) {
		created = true;
		T = Pos;
		R = glm::rotate(glm::mat4(1.0), glm::radians(-angle), glm::vec3(1.0, 0.0, 0.0));
	}

	void Move();
	~Thorn(){}
};