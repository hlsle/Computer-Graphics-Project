#include "Map.h"
void Map::Move(int dir) {
	if (dir == 1) {
		T = glm::translate(T, glm::vec3(-0.1, 0.0, -0.1));
	}
	else{
		T = glm::translate(T, glm::vec3(0.1, 0.0, -0.1));
	}
}