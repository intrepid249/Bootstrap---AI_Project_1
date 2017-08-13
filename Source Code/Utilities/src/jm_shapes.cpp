#include "jm_shapes.h"

namespace jm {
	Rect::Rect(float _x, float _y, float _width, float _height) {
		x = _x;
		y = _y;
		width = _width;
		height = _height;
	}

	bool Rect::containsPoint(float px, float py) {
		// Coordinates are anchored top-left
		float mx = x + width / 2, my = y + height / 2;

		if (px < mx - width / 2 || px > mx + width / 2) return false;
		if (py < my - height / 2 || py > my + height / 2) return false;

		return true;
	}



	Ray::Ray(glm::vec2 origin, glm::vec2 direction) : m_origin(origin), m_direction(direction) {
	}
}