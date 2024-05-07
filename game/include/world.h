#pragma once

#include <uriel.h>
#include <vector>

#include "tile.h"

class Tile;

class World {
public:
	World();
	World(const Sint64 width, const Sint64 height, const std::vector<Tile> tiles);
	void displayTile(const Uint64 x, const Uint64 y);

	// World is only designed for even widths and heights
	Sint64 width;
	Sint64 height;
	std::vector<Tile> tiles;

private:
	Sint64 horizontalOffset;
	Sint64 verticalOffset;
};

World loadWorldFromFile(const std::string &filepath);
