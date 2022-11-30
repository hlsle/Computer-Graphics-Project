#pragma once
#include "objRead.h"
class Map {
public:
	glm::mat4 T = glm::mat4(1.0);
	glm::mat4 S = glm::mat4(1.0);
	glm::mat4 R = glm::mat4(1.0);
	float Pos_z = 0.0;
	int dir = 1;
	float speed = 0.5;

	Map(){}
	void init_Pos(glm::vec3 Pos, float angle, float z) {
		Pos_z = z;
		T = glm::translate(T, Pos);
		R = glm::rotate(R, glm::radians(-angle), glm::vec3(1.0, 0.0, 0.0));
	}

	void Move();

	~Map() {}
};