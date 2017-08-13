#include "jm_utilities.h"

#include <algorithm>

namespace jm {
	void stringSplit(std::string &s, char delim, std::vector<std::string> &v) {
		auto i = 0;
		auto pos = s.find(delim);
		while (pos != std::string::npos) {
			v.push_back(s.substr(i, pos - i));
			i = ++pos;
			pos = s.find(delim, pos);

			if (pos == std::string::npos) {
				v.push_back(s.substr(i, s.length()));
			}
		}
	}

	float degToRad(float degrees) {
		return float(degrees * (M_PI / 180.f));
	}

	float radToDeg(float radians) {
		return float(radians * (180.f / M_PI));
	}
	float clampf(const float val, const float min, const float max) {
		return std::max(min, std::min(val, max));
	}
}