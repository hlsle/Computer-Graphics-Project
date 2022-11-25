#pragma once
#include "objRead.h"
class Map {
public:
	glm::mat4 T = glm::mat4(1.0);
	glm::mat4 S = glm::mat4(1.0);
	glm::mat4 R = glm::mat4(1.0);

	Map(float pos, float angle) {
		T = glm::translate(T, glm::vec3(0.0, 0.0, -pos));
		R = glm::rotate(R, glm::radians(-angle), glm::vec3(1.0, 0.0, 0.0));
	}

	void Move(int dir);

	~Map() {}
};