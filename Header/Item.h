#pragma once
#include "objRead.h"

class Point {
public:
	glm::mat4 S = glm::mat4(1.0f);
	glm::mat4 R = glm::mat4(1.0f);
	glm::mat4 T = glm::mat4(1.0f);

	int point;

	Point(glm::vec3 a, float r) {//생성 위치, 맵의 기울기
		R = glm::rotate(R, glm::radians(r), glm::vec3(1.0f, 0.0f, 0.1f));
		T = glm::translate(T, a);

		point = 100;
	}
	~Point() {

	}

	void move();
	int pluspoint(int a);
};
class Heal {
public:
	glm::mat4 S = glm::mat4(1.0f);
	glm::mat4 R = glm::mat4(1.0f);
	glm::mat4 T = glm::mat4(1.0f);

	int heal;

	Heal(glm::vec3 a, float r) {//생성 위치, 맵의 기울기
		R = glm::rotate(R, glm::radians(r), glm::vec3(1.0f, 0.0f, 0.1f));
		T = glm::translate(T, a);

		heal = 1;
	}
	~Heal() {

	}

	void move();
	int plushp(int a);
};
class Shield {
	glm::mat4 S = glm::mat4(1.0f);
	glm::mat4 R = glm::mat4(1.0f);
	glm::mat4 T = glm::mat4(1.0f);

	int shield;

	Shield(glm::vec3 a, float r) {//생성 위치, 맵의 기울기
		R = glm::rotate(R, glm::radians(r), glm::vec3(1.0f, 0.0f, 0.1f));
		T = glm::translate(T, a);

		shield = 1;
	}
	~Shield() {

	}

	void move();
	int plusshield(int a);

};