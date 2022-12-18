#include "Ob.h"
void Rock::Move() {

		T.z += speed;

}
void Rock::DirMove() {
	if (dir == 1) {
		T.x -= 0.1;
	}
	else {
		T.x += 0.1;
	}
}

void Bomb::Move() {

		T.z += speed;
	
}
void Bomb::DirMove() {
	if (dir == 1) {
		T.x -= 0.1;
	}
	else {
		T.x += 0.1;
	}
}

void Thorn::Move() {
	
		T.z += speed;
	

}
void Thorn::DirMove() {
	if (dir == 1) {
		T.x -= 0.1;
	}
	else {
		T.x += 0.1;
	}
}

void Jumpbar::Move() {

		T.z += speed;

}
void Jumpbar::DirMove() {
	if (dir == 1) {
		T.x -= 0.1;
	}
	else {
		T.x += 0.1;
	}
}