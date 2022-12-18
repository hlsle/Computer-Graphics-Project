#pragma once
#include "Camera.h"

glm::mat4 Camera::displayCamera() {
	return glm::lookAt(cameraPos, cameraDir, cameraUp);
}