#pragma once
#include "Item.h"

void Point::move() {

}
int Point::pluspoint(int a) {
	return a + point;
}

void Heal::move() {

}
int Heal::plushp(int a) {
	return a + heal;
}

void Shield::move() {

}
int Shield::plusshield(int a) {
	if (a != 0)
		return a;
	else
		return a + shield;
}

