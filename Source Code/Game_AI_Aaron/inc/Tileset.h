#pragma once
#include <string>

namespace aie {
	class Texture;
}

namespace jm {
	struct Tileset {
		Tileset(unsigned int _firstGid, std::string _name, unsigned int _tileWidth, unsigned int _tileHeight,
			unsigned int _tileCount, unsigned int _columns,
			unsigned int _imgWidth, unsigned int _imgHeight) {
			firstGID = _firstGid;
			name = _name.c_str();
			tileWidth = _tileWidth;
			tileHeight = _tileHeight;
			tileCount = _tileCount;
			columns = _columns;
			imageWidth = _imgWidth;
			imageHeight = _imgHeight;
		}

		unsigned int firstGID;
		const char * name;
		unsigned int tileWidth;
		unsigned int tileHeight;
		unsigned int tileCount;
		unsigned int columns;
		unsigned int imageWidth;
		unsigned int imageHeight;
	};

	struct TileLayer {
		TileLayer(std::string name, unsigned int width, unsigned int height) {
			this->name = name;
			this->width = width;
			this->height = height;
		}

		std::string name;
		unsigned int width;
		unsigned int height;
		std::vector<std::string> layerData;
	};

	struct Tile {
		int layerDepth;
		aie::Texture *image;
		int gid;
		float width;
		float height;
		int uv_x;
		int uv_y;
	};

	struct Object {
		Object(float _x, float _y, float _width, float _height) {
			x = _x;
			y = _y;
			width = _width;
			height = _height;
		}

		float x;
		float y;
		float width;
		float height;
	};
}