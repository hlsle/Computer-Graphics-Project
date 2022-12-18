#pragma once
#include "objRead.h"

class Point {
public:
	glm::mat4 R = glm::mat4(1.0f);
	glm::vec3 T = glm::vec3(0.0f, 0.0f, 0.0f);
	bool created = false;
	int dir = 1;
	float speed = 0.5;

	Point() {//생성 위치, 맵의 기울기
	}
	void init_pos(glm::vec3 a, float r) {
		created = true;
		R = glm::rotate(R, glm::radians(-r), glm::vec3(1.0f, 0.0f, 0.0f));
		T = a;
	}
	~Point() {

	}

	void Move();
	void DirMove();
};
class Heal {
public:
	glm::mat4 R = glm::mat4(1.0f);
	glm::vec3 T = glm::vec3(0.0f, 0.0f, 0.0f);
	bool created = false;
	int dir = 1;
	float speed = 0.5;

	Heal() {//생성 위치, 맵의 기울기
	}
	void init_pos(glm::vec3 a, float r) {
		created = true;
		R = glm::rotate(R, glm::radians(r), glm::vec3(1.0f, 0.0f, 0.0f));
		T = a;
	}
	~Heal() {

	}

	void Move();
	void DirMove();
};
class Shield {
public:
	glm::mat4 R = glm::mat4(1.0f);
	glm::vec3 T = glm::vec3(0.0f, 0.0f, 0.0f);
	bool created = false;
	int dir = 1;
	float speed = 0.5;

	Shield() {//생성 위치, 맵의 기울기
	}
	void init_pos(glm::vec3 a, float r) {
		created = true;
		R = glm::rotate(R, glm::radians(r), glm::vec3(1.0f, 0.0f, 0.0f));
		T = a;
	}
	~Shield() {

	}

	void Move();
	void DirMove();
};