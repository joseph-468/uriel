#include <uriel.h>
#include <string>
#include <fstream>
#include <algorithm>

#include "../include/game.h"
#include "../include/world.h"

using namespace Uriel;

World::World() : width(0), height(0), horizontalOffset(0), verticalOffset(0) {}

World::World(const Sint64 width, const Sint64 height, const std::vector<Tile> tiles) : width(width), height(height), tiles(tiles) {
	horizontalOffset = TILE_SIZE / 2 + (width / 2 * -TILE_SIZE);
	verticalOffset = (height / 2 * TILE_SIZE) - TILE_SIZE / 2;
}

void World::displayTile(const Uint64 x, const Uint64 y) {
	Tile &tile = tiles[y * width + x];
	if (tile.typeId == 0) return;

	Sint64 xPos = horizontalOffset + x * TILE_SIZE;
	Sint64 yPos = verticalOffset - y * TILE_SIZE;
	drawSprite(tile.spriteId, static_cast<float>(xPos), static_cast<float>(yPos), TILE_SIZE, TILE_SIZE);
}

void World::displayTiles(const Camera &camera) {
	Sint64 top = static_cast<Sint64>(floor(height / 2 - (camera.y + (camera.height / 2)) / TILE_SIZE));
	Sint64 bottom = static_cast<Sint64>(floor(height / 2 - (camera.y - (camera.height / 2)) / TILE_SIZE));
	Sint64 left = static_cast<Sint64>(floor(width / 2 + (camera.x - (camera.width / 2)) / TILE_SIZE));
	Sint64 right = static_cast<Sint64>(floor(width / 2 + (camera.x + (camera.width / 2)) / TILE_SIZE));

	top = std::clamp(top, static_cast<Sint64>(0), height - 1);
	bottom  = std::clamp(bottom, static_cast<Sint64>(0), height - 1);
	left = std::clamp(left, static_cast<Sint64>(0), width - 1);
	right = std::clamp(right, static_cast<Sint64>(0), width - 1);

	if (top == bottom || left == right) return;
	for (Sint64 y = top; y <= bottom; y++) {
		for (Sint64 x = left; x <= right; x++) {
			displayTile(x, y);
		}
	}
}

World loadWorldFromFile(const std::string &filepath) {
	std::ifstream file(filepath);
	std::string data;

	getline(file, data);
	Sint64 width = stoi(data);
	getline(file, data);
	Sint64 height = stoi(data);

	std::vector<Tile> tiles;
	getline(file, data);
	for (char ch : data) {
		tiles.push_back(Tile(ch - '0', ch - '0'));
	}

	return World(width, height, tiles);
}

World generateWorld() {
	std::vector<Tile> tiles;
	for (int y = 0; y < 64; y++) {
		for (int x = 0; x < 256; x++) {
			int id = 0;
			if (y == 33) id = 2;
			if (y > 33) id = 3;
			if (y > 39) id = 1;
			tiles.push_back(Tile(id , id));
		}
	}
	
	return World(256, 64, tiles);
}

