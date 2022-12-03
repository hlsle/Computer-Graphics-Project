#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <deque>

#include <time.h>
#include "Camera.h"
#include "Character.h"
#include "Item.h"
#include "Map.h"
#include "Ob.h"

using namespace std;

struct objRead {

	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;
	std::vector< glm::vec3 > outvertex, outnormal;
	std::vector< glm::vec2 > outuv;

	float sumX = 0.0, sumY = 0.0, sumZ = 0.0;
	float aveX = 0.0, aveY = 0.0, aveZ = 0.0;
	float scaleX = 0.0, scaleY = 0.0, scaleZ = 0.0;
	float minX = 0.0, minY = 0.0, minZ = 0.0;
	float maxX = 0.0, maxY = 0.0, maxZ = 0.0;
	float scaleAll;

	float sizeX = 0.0, sizeY = 0.0, sizeZ = 0.0;

	int loadObj_normalize_center(const char* filename);

};
