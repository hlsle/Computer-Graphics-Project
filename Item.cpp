#pragma once
#include "Item.h"

void Point::Move() {
	if (dir == 1) {
		T.x -= 0.1;
		T.z += speed;
	}
	else {
		T.x += 0.1;
		T.z += speed;
	}
}
void Heal::Move() {
	if (dir == 1) {
		T.x -= 0.1;
		T.z += speed;
	}
	else {
		T.x += 0.1;
		T.z += speed;
	}
}

void Shield::Move() {
	if (dir == 1) {
		T.x -= 0.1;
		T.z += speed;
	}
	else {
		T.x += 0.1;
		T.z += speed;
	}
}

