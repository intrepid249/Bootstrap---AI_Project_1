#pragma once
#include <glm\vec2.hpp>

void stringSplit() {}

void truncate(glm::vec2 &vec, float max) {
	float sFac = max / vec.length();
	sFac = sFac < 1.0f ? sFac : 1.0f;
	vec *= sFac; // scale the vector by the factor
}