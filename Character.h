#pragma once
#include "objRead.h"
class Character {
public:
	glm::mat4 T = glm::mat4(1.0);
	glm::mat4 S = glm::mat4(1.0);
	glm::mat4 R = glm::mat4(1.0);

	Character() {
		
	}

	void move_rotate();
	void jump();

	~Character() {}
};