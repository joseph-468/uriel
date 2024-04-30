#include <uriel.h>
#include <string>
#include <fstream>
#include <iostream>

#include "../include/game.h"
#include "../include/world.h"

using namespace Uriel;

World::World() : width(0), height(0), horizontalOffset(0), verticalOffset(0) {}

World::World(const Sint64 width, const Sint64 height, const std::vector<Tile> tiles) : width(width), height(height), tiles(tiles) {
	horizontalOffset = Game::TILE_SIZE / 2 + (width / 2 * -Game::TILE_SIZE);
	verticalOffset = (height / 2 * Game::TILE_SIZE) - Game::TILE_SIZE / 2;
}

void World::displayTile(const Uint64 x, const Uint64 y) {
	// Textures are temp
	static SpriteSheet tilesSheet("assets/tiles.png");
	static Sprite air(tilesSheet, { 0, 0, 8, 8});
	static Sprite cobble(tilesSheet, { 8, 0, 8, 8});
	static Sprite dirt(tilesSheet, { 0, 8, 8, 8});
	static Sprite grass(tilesSheet, {8, 8, 8, 8});

	Tile &tile = tiles[y * width + x];
	if (tile.typeID == 0) return;

	Sint64 xPos = horizontalOffset + x * Game::TILE_SIZE;
	Sint64 yPos = verticalOffset - y * Game::TILE_SIZE;
	if (tile.typeID == 1) drawSprite(cobble, xPos, yPos, Game::TILE_SIZE, Game::TILE_SIZE);
	if (tile.typeID == 2) drawSprite(dirt, xPos, yPos, Game::TILE_SIZE, Game::TILE_SIZE);
	if (tile.typeID == 3) drawSprite(grass, xPos, yPos, Game::TILE_SIZE, Game::TILE_SIZE);
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
		tiles.push_back(Tile(ch - '0'));
	}

	return World(width, height, tiles);
}
