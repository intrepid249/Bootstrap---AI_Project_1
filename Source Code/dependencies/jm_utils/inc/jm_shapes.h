#pragma once
#include <glm\vec2.hpp>

namespace jm {
	struct Ray {
		Ray(glm::vec2 origin, glm::vec2 direction);

		glm::vec2 m_origin, m_direction;
	};

	struct Rect {
		Rect(float _x, float _y, float _width, float _height);

		bool containsPoint(float x, float y);

		float x;
		float y;
		float width;
		float height;
	};
}