#pragma once
#include "objRead.h"
class Rock {
public:
	glm::mat4 T = glm::mat4(1.0);
	glm::mat4 S = glm::mat4(1.0);
	glm::mat4 R = glm::mat4(1.0);
	Rock(float x, float z, float angle){
		T = glm::translate(T, glm::vec3(x, 0.0, z));
		R = glm::rotate(R, glm::radians(-angle), glm::vec3(1.0, 0.0, 0.0));
	}
	
	void Move(float z);
	~Rock(){}
};

class Bomb {
public:
	glm::mat4 T = glm::mat4(1.0);
	glm::mat4 S = glm::mat4(1.0);
	glm::mat4 R = glm::mat4(1.0);
	Bomb(float x, float z, float angle){
		T = glm::translate(T, glm::vec3(x, 0.0, z));
		R = glm::rotate(R, glm::radians(-angle), glm::vec3(1.0, 0.0, 0.0));
	}

	void Move(float z);
	~Bomb(){}
};

class  Thorn{
public:
	glm::mat4 T = glm::mat4(1.0);
	glm::mat4 S = glm::mat4(1.0);
	glm::mat4 R = glm::mat4(1.0);
	Thorn(float x, float z, float angle){
		T = glm::translate(T, glm::vec3(x, 0.0, z));
		R = glm::rotate(R, glm::radians(-angle), glm::vec3(1.0, 0.0, 0.0));
	}

	void Move(float z);
	~Thorn(){}
};