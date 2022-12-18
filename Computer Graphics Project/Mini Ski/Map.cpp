#include "Map.h"
void Map::Move() {

		T.z += speed;

}
void Map::DirMove() {
	if (dir == 1) {
		T.x -= 0.1;
	}
	else {
		T.x += 0.1;
	}
}