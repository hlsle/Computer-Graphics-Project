#include "Ob.h"
void Rock::Move() {
	if (dir == 1) {
		T.x -= 0.1;
		T.z += speed;
	}
	else {
		T.x += 0.1;
		T.z += speed;
	}
}

void Bomb::Move() {
	if (dir == 1) {
		T.x -= 0.1;
		T.z += speed;
	}
	else {
		T.x += 0.1;
		T.z += speed;
	}
}

void Thorn::Move() {
	if (dir == 1) {
		T.x -= 0.1;
		T.z += speed;
	}
	else {
		T.x += 0.1;
		T.z += speed;
	}
}