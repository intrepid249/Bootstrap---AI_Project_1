#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <string>

namespace jm {

	void stringSplit(std::string &s, char delim, std::vector<std::string> &v);

	float degToRad(float degrees);

	float radToDeg(float radians);

	float clampf(const float val, const float min, const float max);
}