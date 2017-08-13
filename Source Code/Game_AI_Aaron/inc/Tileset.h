#pragma once
#include <string>

namespace aie {
	class Texture;
}

namespace jm {
	struct Tileset {
		Tileset(int _firstGid, std::string _name, int _tileWidth, int _tileHeight,
			int _tileCount, int _columns,
			int _imgWidth, int _imgHeight) {
			firstGID = _firstGid;
			name = _name.c_str();
			tileWidth = _tileWidth;
			tileHeight = _tileHeight;
			tileCount = _tileCount;
			columns = _columns;
			imageWidth = _imgWidth;
			imageHeight = _imgHeight;
		}

		int firstGID;
		const char * name;
		int tileWidth;
		int tileHeight;
		int tileCount;
		int columns;
		int imageWidth;
		int imageHeight;
	};

	struct TileLayer {
		TileLayer(std::string name, int width, int height) {
			this->name = name;
			this->width = width;
			this->height = height;
		}

		std::string name;
		int width;
		int height;
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
}