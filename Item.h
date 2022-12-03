#pragma once
#include "objRead.h"

class Point {
public:
	glm::mat4 R = glm::mat4(1.0f);
	glm::vec3 T = glm::vec3(0.0f, 0.0f, 0.0f);
	bool created = false;
	bool use = false;
	int dir = 1;
	float speed = 0.5;

	Point() {//���� ��ġ, ���� ����
	}
	void init_pos(glm::vec3 a, float r) {
		created = true;
		use = true;
		R = glm::rotate(R, glm::radians(-r), glm::vec3(1.0f, 0.0f, 0.0f));
		T = a;
	}
	~Point() {

	}

	void Move();
};
class Heal {
public:
	glm::mat4 R = glm::mat4(1.0f);
	glm::vec3 T = glm::vec3(0.0f, 0.0f, 0.0f);
	bool created = false;
	bool use = false;
	int dir = 1;
	float speed = 0.5;

	Heal() {//���� ��ġ, ���� ����
	}
	void init_pos(glm::vec3 a, float r) {
		created = true;
		use = true;
		R = glm::rotate(R, glm::radians(r), glm::vec3(1.0f, 0.0f, 0.0f));
		T = a;
	}
	~Heal() {

	}

	void Move();
};
class Shield {
public:
	glm::mat4 R = glm::mat4(1.0f);
	glm::vec3 T = glm::vec3(0.0f, 0.0f, 0.0f);
	bool created = false;
	bool use = false;
	int dir = 1;
	float speed = 0.5;

	Shield() {//���� ��ġ, ���� ����
	}
	void init_pos(glm::vec3 a, float r) {
		created = true;
		use = true;
		R = glm::rotate(R, glm::radians(r), glm::vec3(1.0f, 0.0f, 0.0f));
		T = a;
	}
	~Shield() {

	}

	void Move();
};