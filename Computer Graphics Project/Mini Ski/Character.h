#pragma once
#include "objRead.h"
class Character {
public:
	glm::vec3 T = glm::vec3(0.0, 0.0, 0.0);

	int point = 0;
	int hp = 3;
	bool has_shield = false;
	Character() {
		T.z = 3.0;
	}

	void jump(float height);

	~Character() {}
};