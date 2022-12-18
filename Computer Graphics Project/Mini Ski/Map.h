#pragma once
#include "Ob.h"
class Map {
public:
	glm::vec3 T = glm::vec3(0.0, 0.0, 0.0);
	glm::mat4 R = glm::mat4(1.0);
	int dir = 1;
	float speed = 0.5;

	Map(){}
	void init_Pos(glm::vec3 Pos, float angle) {
		T = Pos;
		R = glm::rotate(R, glm::radians(-angle), glm::vec3(0.0, 1.0, 0.0));
	}

	void Move();
	void DirMove();
	~Map() {}
};