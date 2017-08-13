#pragma once

namespace jm {
	struct Rect {
		Rect(float _x, float _y, float _width, float _height);

		float x;
		float y;
		float width;
		float height;
	};
}