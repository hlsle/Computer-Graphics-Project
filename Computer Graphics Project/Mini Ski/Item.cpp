#pragma once
#include "Item.h"

void Point::Move() {

		T.z += speed;

}
void Point::DirMove() {
	if (dir == 1) {
		T.x -= 0.1;
	}
	else {
		T.x += 0.1;
	}
}
void Heal::Move() {

		T.z += speed;
	

}
void Heal::DirMove() {
	if (dir == 1) {
		T.x -= 0.1;
	}
	else {
		T.x += 0.1;
	}
}
void Shield::Move() {

		T.z += speed;
	

}
void Shield::DirMove() {
	if (dir == 1) {
		T.x -= 0.1;
	}
	else {
		T.x += 0.1;
	}
}
