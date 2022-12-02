#pragma once
#include "objRead.h"
class Character {
public:
	glm::vec3 T = glm::vec3(0.0, 0.0, 0.0);
	glm::mat4 R = glm::mat4(1.0);

	Character() {
		T.z = 3.0;
	}

	void move_rotate();
	void jump();

	~Character() {}
};