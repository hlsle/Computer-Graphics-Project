#include "Map.h"
void Map::Move() {
	if (dir == 1) {
		T = glm::translate(T, glm::vec3(-0.1, 0.0, speed));
	}
	else{
		T = glm::translate(T, glm::vec3(0.1, 0.0, speed));
	}
}