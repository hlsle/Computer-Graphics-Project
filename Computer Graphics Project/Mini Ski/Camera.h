#pragma once
#include "objRead.h"

class Camera {
	glm::vec3 cameraPos;
	glm::vec3 cameraDir;
	glm::vec3 cameraUp;
	
	Camera(glm::vec3 pos, glm::vec3 dir) {//, 
		cameraPos = pos;
		cameraDir = dir;
		cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	}
	~Camera() {

	}

	glm::mat4 displayCamera();
};