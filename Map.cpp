#include "Map.h"
void Map::Move() {
	if (dir == 1) {
		T.x -= 0.1;
		T.z += speed;
	}
	else{
		T.x += 0.1;
		T.z += speed;
	}
}