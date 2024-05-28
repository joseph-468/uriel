#pragma once

#include <uriel.h>
#include <vector>
#include <unordered_map>

#include "world.h"

class World;
class TileType;

extern std::vector<TileType> tileTypes;
extern std::unordered_map<std::string, Uint16> tileTypesMap;

class TileType {
public:
	const std::string id;
	const Uint16 spriteId;
	const Uint8 width;
	const Uint8 height;
};

Uint16 getTileTypeIndex(const std::string &name);

void createTileType(const std::string id, const Uint16 spriteId, const Uint8 width, const Uint8 height);

class Tile {
public:
	Tile();
	Tile(const Uint16 typeId);

	Uint16 typeId;
};

bool isCollidingWithTile(const World &world, SDL_FRect target, float collisionTolerance);
